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

  Client(frontend::Gui& gui, uint16_t port);
  ~Client();
  // Despite invoked by render loop, I'm still gonna name it "Update".
  void Update();
  // Use an explicit stop to put it before the window destroied. Otherwise if
  // someday a background buggy thread blocks after the main window closed,
  // it leaves a zombie background process that really hard to be noticed.
  void Stop();
  void GetState();

 private:
  // Limit maximum pending state getting requests.
  static constexpr int kMaxGetState = 1;
  frontend::Gui& gui_;
  httplib::Client cli_;
  httplib::ThreadPool thread_pool_;
  yrtr::TaskQueue render_loop_ch_;
  std::atomic<int> get_state_count_;

  void SendGetState();
  void ParseState(const std::string& data);
  void SendPostInput(FnLabel label, uint32_t val);
  void SendPostButton(FnLabel label);
  void SendPostCheckbox(FnLabel label, bool activate);
  void SendPostProtectedList(SideMap&& side_map);
  void SendPostData(std::string_view path, std::string&& data);

  DISALLOW_COPY_AND_ASSIGN(Client);
};

}  // namespace yrtr
