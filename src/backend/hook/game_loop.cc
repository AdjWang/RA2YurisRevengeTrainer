#include "backend/hook/game_loop.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "GameOptionsClass.h"
#include "TacticalClass.h"
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
static std::unique_ptr<Trainer> trainer;
static std::unique_ptr<Server> server;

static void DoDestroyWindow() {
  // Reclaim resources here.
  server.reset();
  trainer.reset();
}

// int __thiscall CreateWindow_777C30(HINSTANCE hInstance, int xRight, int
// yBottom)
static void PreCreateWindow(HINSTANCE hInstance) {
  // Setup thread id.
  SetupGameLoopThreadOnce();
  // Load configurations.
  char dll_path[MAX_PATH] = {0};
  DWORD nSize = GetModuleFileNameA(hInstance, dll_path, _countof(dll_path));
  if (nSize == 0) {
    DWORD err = GetLastError();
    std::string message = std::system_category().message(err);
    LOG_F(FATAL, "Failed to get module file name, err=[{}]{}", err, message);
    UNREACHABLE();
  }
  // Search configuration file at the same directory with the dll.
  CHECK(Config::Load(fs::canonical(fs::path(dll_path)).parent_path()));
  trainer = std::make_unique<Trainer>();
  server = std::make_unique<Server>(trainer.get(), Config::instance()->port());
}

static void __declspec(naked) __cdecl InjectCreateWindow() {
  static const uint32_t jmp_back = GetJumpBack(kHpCreateWindow);
  static HINSTANCE hInstance;  // from WinMain
  __asm {
    mov [hInstance], ecx
    pushad
  }
  PreCreateWindow(hInstance);
  __asm {
    popad
    sub esp, 0x38
    push ebx
    push ebp
    jmp [jmp_back]
  }
}
static void Update() {
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
static void __declspec(naked) __cdecl InjectUpdate() {
  static const uint32_t jmp_back = GetJumpBack(kHpUpdate);
  __asm {
    pushad
  }
  Update();
  __asm {
    popad
    mov eax, 0x00647260 // LoopOnce
    call eax
    jmp [jmp_back]
  }
}

static void ExitGame() {
  LOG_F(INFO, "Manually send destroy event");
  DestroyWindowOnce();
  auto ra2WndProc = reinterpret_cast<WNDPROC>(0x007775C0);
  CHECK_NOTNULL(ra2WndProc);
  HWND hWnd = *(reinterpret_cast<HWND*>(0x00B73550));
  CallWindowProc(ra2WndProc, hWnd, WM_DESTROY, NULL, NULL);
}
static void __declspec(naked) __cdecl InjectExitGame() {
  static const uint32_t jmp_back = GetJumpBack(kHpExitGame);
  __asm {
    pushad
  }
  ExitGame();
  __asm {
    popad
    jmp [jmp_back]
  }
}
}  // namespace

void HookCreateWindow() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpCreateWindow, InjectCreateWindow);
}

void DestroyWindowOnce() {
  static std::once_flag destroy_flag;
  std::call_once(destroy_flag, []() { DoDestroyWindow(); });
}

void HookUpdate() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpUpdate, InjectUpdate);
}

void HookExitGame() {
  DLOG_F(INFO, "[YRTR-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpExitGame, InjectExitGame);
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
