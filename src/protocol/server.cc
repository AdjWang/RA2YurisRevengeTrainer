#include "protocol/server.h"

#include "base/logging.h"
#include "base/thread.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace yrtr {

Server::Server(backend::hook::ITrainer* trainer, uint16_t port)
    : trainer_(trainer) {
  game_loop_ch_.SetThreadId(GetGameLoopThreadId());
  // This tool is used in LAN, 50ms should be enough. Large timeout queues too
  // many jobs in sending queue when the backend is not running, making the gui
  // actions, like close window, acts lagging.
  svr_.set_open_handshake_timeout(50);
  svr_.set_close_handshake_timeout(50);
  svr_.set_pong_timeout(200);
  svr_.set_access_channels(websocketpp::log::alevel::all);
  svr_.clear_access_channels(websocketpp::log::alevel::frame_payload);
  svr_.init_asio();
  svr_.set_message_handler([this](websocketpp::connection_hdl hdl,
                                  WebsocketServer::message_ptr msg) {
    OnWebsocketMessage(svr_, hdl, msg);
  });
  svr_.listen(port);
  svr_.start_accept();
  evloop_ = std::thread([this]() { svr_.run(); });
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

void Server::OnWebsocketMessage(WebsocketServer& svr,
                                websocketpp::connection_hdl hdl,
                                WebsocketServer::message_ptr msg) {
  LOG(INFO) << "on_message called with hdl: " << hdl.lock().get()
            << " and message: " << msg->get_payload();
  UNREFERENCED_PARAMETER(svr);
  // try {
  //   svr.send(hdl, msg->get_payload(), msg->get_opcode());
  // } catch (websocketpp::exception const& e) {
  //   LOG(ERROR) << "Echo failed because: "
  //              << "(" << e.what() << ")";
  // }
}

// void Server::OnGetState(const httplib::Request& /*req*/,
//                         httplib::Response& res) {
//   DCHECK(IsWithinNetThread());
//   State state = trainer_->state();
//   std::string data = json(state).dump();
//   res.set_content(data, "application/json");
// }

// void Server::OnPostEvent(const httplib::Request& req, httplib::Response& res) {
//   DCHECK(IsWithinNetThread());
//   try {
//     json raw_data = json::parse(req.body);
//     std::string type = raw_data.at("type");
//     if (type == "input") {
//       OnPostInputEvent(ParseInputEvent(raw_data));
//     } else if (type == "button") {
//       OnPostButtonEvent(ParseButtonEvent(raw_data));
//     } else if (type == "checkbox") {
//       OnPostCheckboxEvent(ParseCheckboxEvent(raw_data));
//     } else if (type == "protected_list") {
//       OnPostProtectedListEvent(ParseProtectedListEvent(raw_data));
//     } else {
//       LOG_F(ERROR, "Unknown event type={} from data={}", type, req.body);
//     }
//   } catch (const std::exception& e) {
//     LOG_F(ERROR, "Failed to parse json data={} error={}", req.body, e.what());
//   }
//   res.set_content("{}", "application/json");
// }

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
