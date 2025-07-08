#include "backend/hook/game_loop.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "GameOptionsClass.h"
#include "TacticalClass.h"
#include "Unsorted.h"
#include "WWMouseClass.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "backend/config.h"
#include "backend/hook/hook_point.h"
#include "backend/hook/memory_api.h"
#include "backend/hook/trainer.h"
#include "base/logging.h"
#include "base/thread.h"
#include "protocol/server.h"

namespace yrtr {
namespace backend {
namespace hook {

namespace {
static std::once_flag init_once;
static std::unique_ptr<Trainer> trainer;
static std::unique_ptr<Server> server;

static void ReclaimResource() {
  DLOG_F(INFO, "Reclaim resources");
  server->Stop();
  server.reset();
  trainer.reset();
}

// int __thiscall CreateWindow_777C30(HINSTANCE hInstance, int xRight, int
// yBottom)
static void Init(HINSTANCE hInstance) {
  // Setup thread id.
  SetupGameLoopThreadOnce();
  // Load configurations.
  char exe_path[MAX_PATH] = {0};
  DWORD nSize = GetModuleFileNameA(hInstance, exe_path, _countof(exe_path));
  if (nSize == 0) {
    DWORD err = GetLastError();
    std::string message = std::system_category().message(err);
    LOG_F(FATAL, "Failed to get module file name, err=[{}]{}", err, message);
    UNREACHABLE();
  }
  // Search configuration file at the same directory with the dll.
  CHECK(Config::Load(fs::canonical(fs::path(exe_path)).parent_path()));
  trainer = std::make_unique<Trainer>(Config::instance());
  server = std::make_unique<Server>(trainer.get(), Config::instance()->port());
}

static void Update() {
  std::call_once(init_once, [&]() {
    // https://devblogs.microsoft.com/oldnewthing/20040614-00/?p=38903
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(kImageBase);
    Init(hInstance);
  });
  DCHECK(IsWithinGameLoopThread());
  static std::chrono::system_clock::time_point last_ts =
      std::chrono::system_clock::now();
  auto ts_now = std::chrono::system_clock::now();
  int64_t delta_us =
      std::chrono::duration_cast<std::chrono::microseconds>(ts_now - last_ts)
          .count();
  last_ts = ts_now;
  double delta_sec = static_cast<double>(delta_us) / 10e6;
  DCHECK_NOTNULL(trainer);
  trainer->Update(delta_sec);
  DCHECK_NOTNULL(server);
  server->Update();
}

static DWORD WINAPI InjectTimeGetTime() {
  static uint32_t last_frame = 0;
  if (yrpp::Game::IsActive) {
    uint32_t current_frame = yrpp::Unsorted::CurrentFrame;
    if (current_frame > last_frame) {
      last_frame = current_frame;
      Update();
    }
  }
  // Original code.
  return timeGetTime();
}

static void ExitGame() {
  LOG_F(INFO, "Manually send destroy event");
  ReclaimResourceOnce();
  auto ra2WndProc = reinterpret_cast<WNDPROC>(0x007775C0);
  CHECK_NOTNULL(ra2WndProc);
  HWND hWnd = *(reinterpret_cast<HWND*>(0x00B73550));
  CallWindowProc(ra2WndProc, hWnd, WM_DESTROY, NULL, NULL);
}
static void WINAPI InjectPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam,
                                      LPARAM lParam) {
  if (Msg == WM_DESTROY) [[unlikely]] {
    ExitGame();
  }
  PostMessageA(hWnd, Msg, wParam, lParam);
}
}  // namespace

void ReclaimResourceOnce() {
  static std::once_flag destroy_flag;
  std::call_once(destroy_flag, []() { ReclaimResource(); });
}

void HookUpdate() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->WriteMemory(
      kHpUpdate.first, reinterpret_cast<void*>(InjectTimeGetTime));
}

void HookExitGame() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->WriteMemory(
      kHpExitGame.first, reinterpret_cast<void*>(InjectPostMessageA));
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
