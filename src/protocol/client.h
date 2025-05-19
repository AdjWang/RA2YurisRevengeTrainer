#pragma once
#include <atomic>

#include "base/macro.h"
#include "base/task_queue.h"
#include "frontend/desktop/gui.h"
#include "httplib.h"
#include "protocol/model.h"

namespace yrtr {

class Client {
 public:
  static constexpr int kTimerIdUpdateState = 1;
  static constexpr std::string_view kApiGetState = "/state";
  static constexpr std::string_view kApiPostEvent = "/event";

  Client(frontend::Gui& gui);
  ~Client();
  const State& state() const { return state_; }
  void UpdateState();
  void ServeAsBackend();

 private:
  // Limit maximum pending state getting requests.
  static constexpr int kMaxGetState = 1;
  State state_;
  httplib::Client cli_;
  httplib::ThreadPool thread_pool_;
  yrtr::TaskQueue render_loop_ch_;
  std::atomic<int> get_state_count_;

  void GetState();
  void ParseState(const std::string& data);
  void SendInput(FnLabel label, uint32_t val);
  void SendButton(FnLabel label);
  void SendCheckbox(FnLabel label, bool activate);
  void PostData(std::string_view path, std::string&& data);

  DISALLOW_COPY_AND_ASSIGN(Client);
};

}  // namespace yrtr
