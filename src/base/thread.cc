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

// Game loop thread is only available on backend.
static std::atomic<ThreadId> tid_game_loop = 0;
// Render loop thread is only available on frontend.
static std::atomic<ThreadId> tid_render_loop = 0;
// 1 thread for network should be enough.
static std::atomic<ThreadId> tid_net_loop = 0;
}  // namespace

ThreadId GetGameLoopThreadId() { return tid_game_loop.load(); }
ThreadId GetRendererThreadId() { return tid_render_loop.load(); }
ThreadId GetNetThreadId() { return tid_net_loop.load(); }

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

void SetupNetThreadOnce(ThreadId tid) {
  static std::once_flag once_flag;
  std::call_once(once_flag, [&]() {
    if (tid == 0) {
      tid_net_loop.store(GetCurrentThreadId());
    } else {
      tid_net_loop.store(tid);
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

bool IsWithinNetThread() {
  return tid_net_loop.load() == GetCurrentThreadId();
}

std::string InspectThreads() {
  ThreadId tid_current = GetCurrentThreadId();
  std::stringstream ss;
  ss << "===== Threads: =====\n";
  ss << std::format("Current tid={}\n", tid_current);
  ss << std::format("Game loop tid={}\n", tid_game_loop.load());
  ss << std::format("Renderer tid={}\n", tid_render_loop.load());
  ss << std::format("Net tid={}\n", tid_net_loop.load());
  ss << "====================\n";
  return ss.str();
}

}  // namespace yrtr
