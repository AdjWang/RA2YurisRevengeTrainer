#include "base/thread.h"

#include <atomic>
#include <format>
#include <mutex>
#include <sstream>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down

namespace yrtr {

namespace {
// Not use thread_local because thread ids are passed to other dll modules and
// set in initialize stage to serve some invocation, like SetMenuActive, before
// any of loops initialize its thread local thread id.
static std::atomic<ThreadId> tid_game_loop = 0;
static std::atomic<ThreadId> tid_render_loop = 0;
}  // namespace

ThreadId GetGameLoopThreadId() { return tid_game_loop.load(); }
ThreadId GetRendererThreadId() { return tid_render_loop.load(); }

void SetupGameLoopThreadOnce(ThreadId tid) {
  static std::once_flag once_flag;
  std::call_once(once_flag, [&]() {
    if (tid == 0) {
      tid_game_loop.store(GetCurrentThreadId());
    } else {
      tid_game_loop.store(tid);
    }
  });
}

void SetupRendererThreadOnce(ThreadId tid) {
  static std::once_flag once_flag;
  std::call_once(once_flag, [&]() {
    if (tid == 0) {
      tid_render_loop.store(GetCurrentThreadId());
    } else {
      tid_render_loop.store(tid);
    }
  });
}

bool IsWithinThread(ThreadId tid) {
  return tid == GetCurrentThreadId();
}

bool IsWithinGameLoopThread() {
  return tid_game_loop.load() == GetCurrentThreadId();
}

bool IsWithinRendererThread() {
  return tid_render_loop.load() == GetCurrentThreadId();
}

std::string InspectThreads() {
  ThreadId tid_current = GetCurrentThreadId();
  std::stringstream ss;
  ss << "===== Threads: =====\n";
  ss << std::format("Current tid={}\n", tid_current);
  ss << std::format("Game loop tid={}\n", tid_game_loop.load());
  ss << std::format("renderer tid={}\n", tid_render_loop.load());
  ss << "====================\n";
  return ss.str();
}

}  // namespace yrtr
