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
  // This tool is used in LAN, 50ms should be enough. Large timeout queues too
  // many jobs in sending queue when the backend is not running, making the gui
  // actions, like close window, acts lagging.
  svr_.set_open_handshake_timeout(50);
  svr_.set_close_handshake_timeout(50);
  svr_.set_pong_timeout(200);
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
  svr_.listen(port);
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
      SendState(trainer_->state(), hdl);
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

void Server::OnStateUpdated(State state) {
  DCHECK(IsWithinGameLoopThread());
  boost::asio::dispatch(svr_.get_io_service(), [this, state = state]() {
    DCHECK(IsWithinNetThread());
    for (auto conn : conns_) {
      State state_copy = state;
      SendState(std::move(state_copy), conn);
    }
  });
}

void Server::SendState(State&& state, websocketpp::connection_hdl hdl) {
  DCHECK(IsWithinNetThread());
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
