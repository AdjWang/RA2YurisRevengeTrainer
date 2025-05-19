#include "protocol/server.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"

namespace yrtr {

Server::Server()
    : game_(state_) {
  yrtr::SetupGameLoopThreadOnce(cfg.tid_game_loop);
  yrtr::SetupRendererThreadOnce(cfg.tid_render_loop);
  game_loop_ch_.SetThreadId();
  // TODO
  // net_loop_ch_.SetThreadId();

}

#undef BIND_BUTTON
#undef BIND_BUTTON
#undef BIND_CHECKBOX

void Server::Update(double delta) {
  // if (cfg_.rtenv == yrtr::RuntimeEnv::kGame) {
  //   game_.Update(delta);
  // }
  game_loop_ch_.ExecuteTasks();
}

void Server::ServeAsBackend() {

}

}  // namespace yrtr
