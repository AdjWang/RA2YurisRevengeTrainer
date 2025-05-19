#pragma once

#include "base/macro.h"
#include "base/task_queue.h"
#include "protocol/model.h"

namespace yrtr {

class Server {
 public:
  Server();
  void UpdateState(double /*delta*/);
  void ServeAsBackend();

 private:
  State state_;
  yrtr::TaskQueue game_loop_ch_;
  yrtr::TaskQueue net_loop_ch_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace yrtr
