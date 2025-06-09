#include "protocol/server.h"

#include "base/logging.h"
#include "base/thread.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace yrtr {

Server::Server(backend::hook::ITrainer* trainer, uint16_t port)
    : trainer_(trainer) {
  game_loop_ch_.SetThreadId(GetGameLoopThreadId());
  trainer_->set_on_state_updated(
      std::bind_front(&Server::OnStateUpdated, this));
  svr_.set_open_handshake_timeout(kConnTimeoutMilliseconds);
  svr_.set_close_handshake_timeout(kConnTimeoutMilliseconds);
  svr_.set_pong_timeout(kConnTimeoutMilliseconds * 4);
  svr_.set_access_channels(websocketpp::log::alevel::all);
  svr_.clear_access_channels(websocketpp::log::alevel::frame_payload);
  svr_.init_asio();
  svr_.set_open_handler(
      [this](websocketpp::connection_hdl hdl) { OnOpenConn(hdl); });
  svr_.set_close_handler(
      [this](websocketpp::connection_hdl hdl) { OnCloseConn(hdl); });
  svr_.set_message_handler([this](websocketpp::connection_hdl hdl,
                                  WebsocketServer::message_ptr msg) {
    OnMessage(svr_, hdl, msg);
  });
  svr_.set_http_handler([this](websocketpp::connection_hdl hdl){
    OnHttpRequest(svr_, hdl);
  });
  svr_.listen("0.0.0.0", std::to_string(port));
  svr_.start_accept();
  evloop_ = std::thread([this]() {
    SetupNetThreadOnce();
    svr_.run();
    DLOG_F(INFO, "Server exit");
  });
}

void Server::Stop() {
  boost::asio::dispatch(svr_.get_io_service(), [&]() {
    websocketpp::lib::error_code ec;
    svr_.stop_listening(ec);
    if (ec) {
      LOG_F(ERROR, "Failed to stop listening err=[{}]{}", ec.value(),
            ec.message());
    }
  });
  evloop_.join();
}

void Server::Update() {
  DCHECK(IsWithinGameLoopThread());
  game_loop_ch_.ExecuteTasks();
}

void Server::OnOpenConn(websocketpp::connection_hdl hdl) {
  DCHECK(IsWithinNetThread());
  conns_.insert(hdl);
}

void Server::OnCloseConn(websocketpp::connection_hdl hdl) {
  DCHECK(IsWithinNetThread());
  conns_.erase(hdl);
}

void Server::OnMessage(WebsocketServer& /*svr*/,
                       websocketpp::connection_hdl hdl,
                       WebsocketServer::message_ptr msg) {
  DCHECK(IsWithinNetThread());
  // Handle event message.
  std::string payload = msg->get_payload();
  try {
    json raw_data = json::parse(payload);
    std::string type = raw_data.at("type");
    if (type == "get_state") {
      OnGetStateEvent(hdl);
    } else if (type == "input") {
      OnPostInputEvent(ParseInputEvent(raw_data));
    } else if (type == "button") {
      OnPostButtonEvent(ParseButtonEvent(raw_data));
    } else if (type == "checkbox") {
      OnPostCheckboxEvent(ParseCheckboxEvent(raw_data));
    } else if (type == "protected_list") {
      OnPostProtectedListEvent(ParseProtectedListEvent(raw_data));
    } else {
      LOG_F(ERROR, "Unknown event type={} from data={}", type, payload);
    }
  } catch (const std::exception& e) {
    LOG_F(ERROR, "Failed to parse json data={} error={}", payload, e.what());
  }
}

void Server::OnHttpRequest(WebsocketServer& /*svr*/,
                           websocketpp::connection_hdl hdl) {
  std::string data = R"(
<!DOCTYPE html>
<html>
    <head>
        <title>Example</title>
    </head>
    <body>
        <p>This is an example of a simple HTML page with one paragraph.</p>
    </body>
</html>
  )";
  websocketpp::lib::error_code ec;
  WebsocketServer::connection_ptr conn = svr_.get_con_from_hdl(hdl, ec);
  if (ec) {
    LOG_F(ERROR, "Failed to send state data with error=[{}]{}", ec.value(),
          ec.message());
    return;
  }
  conn->set_body(data);
  conn->set_status(websocketpp::http::status_code::ok);
}

void Server::OnStateUpdated(State state) {
  DCHECK(IsWithinGameLoopThread());
  boost::asio::dispatch(svr_.get_io_service(), [this, state = state]() {
    DCHECK(IsWithinNetThread());
    // Broadcast state to clients.
    for (auto conn : conns_) {
      State state_copy = state;
      SendState(std::move(state_copy), conn);
    }
  });
}

void Server::OnGetStateEvent(websocketpp::connection_hdl hdl) {
  DCHECK(IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, hdl = hdl]() {
    DCHECK(IsWithinGameLoopThread());
    State state = trainer_->state();
    boost::asio::dispatch(
        svr_.get_io_service(),
        [this, hdl = hdl, state = std::move(state)]() mutable {
          DCHECK(IsWithinNetThread());
          SendState(std::move(state), hdl);
        });
  });
}

void Server::SendState(State&& state, websocketpp::connection_hdl hdl) {
  DCHECK(IsWithinNetThread());
  if (hdl.expired()) [[unlikely]] {
    conns_.erase(hdl);
    return;
  }
  std::string data = MakeGetStateEvent(std::move(state));
  websocketpp::lib::error_code ec;
  svr_.send(hdl, data, websocketpp::frame::opcode::value::text, ec);
  if (ec) {
    LOG_F(ERROR, "Failed to send state data with error=[{}]{}", ec.value(),
          ec.message());
  }
}

void Server::OnPostInputEvent(Event<uint32_t>&& event) {
  DCHECK(IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_->OnInputEvent(event.label, event.val);
  });
}

void Server::OnPostButtonEvent(Event<int>&& event) {
  DCHECK(IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_->OnButtonEvent(event.label);
  });
}

void Server::OnPostCheckboxEvent(Event<bool>&& event) {
  DCHECK(IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_->OnCheckboxEvent(event.label, event.val);
  });
}

void Server::OnPostProtectedListEvent(Event<SideMap>&& event) {
  DCHECK(IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask(
      [this, event = std::move(event)]() mutable {
        trainer_->OnProtectedListEvent(std::move(event.val));
      });
}

}  // namespace yrtr
