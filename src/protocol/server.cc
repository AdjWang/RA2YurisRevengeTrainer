#include "protocol/server.h"

#include "base/logging.h"
#include "base/thread.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace yrtr {

Server::Server(backend::hook::Trainer& trainer, uint16_t port)
    : trainer_(trainer) {
  game_loop_ch_.SetThreadId(GetGameLoopThreadId());
  svr_.listen("localhost", port);
  svr_.Get(std::string(kApiGetState),
           std::bind_front(&Server::OnGetState, this));
  svr_.Post(std::string(kApiPostEvent),
            std::bind_front(&Server::OnPostEvent, this));
}

void Server::Stop() {
  svr_.stop();
}

void Server::Update() {
  DCHECK(yrtr::IsWithinGameLoopThread());
  game_loop_ch_.ExecuteTasks();
}

void Server::OnGetState(const httplib::Request& /*req*/,
                        httplib::Response& res) {
  DCHECK(yrtr::IsWithinNetThread());
  State state = trainer_.state();
  std::string data = json(state).dump();
  res.set_content(data, "application/json");
}

void Server::OnPostEvent(const httplib::Request& req, httplib::Response& res) {
  DCHECK(yrtr::IsWithinNetThread());
  try {
    json raw_data = json::parse(req.body);
    std::string type = raw_data.at("type");
    if (type == "input") {
      OnPostInputEvent(ParseInputEvent(raw_data));
    } else if (type == "button") {
      OnPostButtonEvent(ParseButtonEvent(raw_data));
    } else if (type == "checkbox") {
      OnPostCheckboxEvent(ParseCheckboxEvent(raw_data));
    } else if (type == "protected_list") {
      OnPostProtectedListEvent(ParseProtectedListEvent(raw_data));
    } else {
      LOG_F(ERROR, "Unknown event type={} from data={}", type, req.body);
    }
  } catch (const std::exception& e) {
    LOG_F(ERROR, "Failed to parse json data={} error={}", req.body, e.what());
  }
  res.set_content("{}", "application/json");
}

void Server::OnPostInputEvent(Event<uint32_t>&& event) {
  DCHECK(yrtr::IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_.OnInputEvent(event.label, event.val);
  });
}

void Server::OnPostButtonEvent(Event<int>&& event) {
  DCHECK(yrtr::IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_.OnButtonEvent(event.label);
  });
}

void Server::OnPostCheckboxEvent(Event<bool>&& event) {
  DCHECK(yrtr::IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask([this, event = std::move(event)]() {
    trainer_.OnCheckboxEvent(event.label, event.val);
  });
}

void Server::OnPostProtectedListEvent(Event<SideMap>&& event) {
  DCHECK(yrtr::IsWithinNetThread());
  game_loop_ch_.ExecuteOrScheduleTask(
      [this, event = std::move(event)]() mutable {
        trainer_.OnProtectedListEvent(std::move(event.val));
      });
}

}  // namespace yrtr
