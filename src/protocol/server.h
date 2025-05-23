#pragma once
#include <thread>

#include "backend/hook/trainer.h"
#include "base/macro.h"
#include "base/task_queue.h"
#define CPPHTTPLIB_THREAD_POOL_COUNT 1
#include "httplib.h"
#include "protocol/model.h"

namespace yrtr {

class Server {
 public:
  Server(backend::hook::ITrainer* trainer, uint16_t port);
  void Stop();
  void Update();

 private:
  backend::hook::ITrainer* trainer_;
  httplib::Server svr_;
  httplib::ThreadPool thread_pool_;
  std::thread listener_;
  yrtr::TaskQueue game_loop_ch_;

  void OnGetState(const httplib::Request& req, httplib::Response& res);
  void OnPostEvent(const httplib::Request& req, httplib::Response& res);
  void OnPostInputEvent(Event<uint32_t>&& event);
  void OnPostButtonEvent(Event<int>&& event);
  void OnPostCheckboxEvent(Event<bool>&& event);
  void OnPostProtectedListEvent(Event<SideMap>&& event);

  DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace yrtr
