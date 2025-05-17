#include "hook/game_loop.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "GameOptionsClass.h"
#include "TacticalClass.h"
#include "WWMouseClass.h"
#include "base/logging.h"
#include "base/thread.h"
#include "gui/window.h"
#include "hook/hook_point.h"
#include "hook/memory_api.h"
#include "hook/render_loop.h"
#include "imgui_impl_win32.h"
#include "tool_hub.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

namespace yrtr {
namespace hook {

namespace {
static std::function<void(HWND)> on_create_window;
static TaskQueue game_loop_ch;

static void ToggleToolMenu() {
  DCHECK(IsWithinRendererThread());
  static bool enable_pannel = false;
  gui::Window* window = gui::Window::instance();
  ToolHub* tool_hub = ToolHub::instance();
  enable_pannel = !enable_pannel;
  if (window != nullptr) {
    window->SetToolMenu(enable_pannel);
  }
  if (tool_hub != nullptr) {
    tool_hub->SetMenuActive(enable_pannel);
  }
}

// I believe this function always running in the game loop.
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam,
                              LPARAM lParam) {
  auto ra2WndProc = reinterpret_cast<WNDPROC>(0x007775C0);
  CHECK_NOTNULL(ra2WndProc);
  static bool is_mouse_captured = false;
  yrpp::WWMouseClass* mouse_class = yrpp::WWMouseClass::Instance();
  gui::Window* window = gui::Window::instance();

  switch (msg) {
    case WM_KEYDOWN: {
      // Ctrl+` was pressed
      if (wParam == VK_OEM_3 && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
        if ((lParam & (1 << 30)) == 0) {
          GetRenderLoopChannel().ExecuteOrScheduleTask(ToggleToolMenu);
          return 0;
        }
      }
      // Fallthrough to ra2.
      return ra2WndProc(hWnd, msg, wParam, lParam);
    }
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
      GetRenderLoopChannel().ExecuteOrScheduleTask(
          [hWnd, msg, wParam, lParam]() {
            DCHECK(IsWithinRendererThread());
            // Always pass mouse move event to imgui to update mouse interacting
            // state.
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
          });
      if (window != nullptr && mouse_class != nullptr) {
        // Update mouse overlay.
        if (is_mouse_captured && window->is_gui_mouse_interacting()) {
          mouse_class->ReleaseMouse();
          is_mouse_captured = false;
        } else if (!is_mouse_captured && !window->is_gui_mouse_interacting()) {
          mouse_class->CaptureMouse();
          is_mouse_captured = true;
        }
      }
      return ra2WndProc(hWnd, msg, wParam, lParam);
    default:
      if (window != nullptr) {
        // Update event path.
        if (window->is_gui_mouse_interacting()) {
          GetRenderLoopChannel().ExecuteOrScheduleTask(
              [hWnd, msg, wParam, lParam]() {
                DCHECK(IsWithinRendererThread());
                ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
              });
          return 0;
        }
      }
      return ra2WndProc(hWnd, msg, wParam, lParam);
  }
}

static void DoDestroyWindow() {
  gui::Window::Destroy();
  ToolHub::Destroy();
}

// int __thiscall CreateWindow_777C30(HINSTANCE hInstance, int xRight, int
// yBottom)
static void CreateWindowImpl(HINSTANCE hInstance, int width, int height,
                             int nShowCmd) {
  // Setup thread id.
  SetupGameLoopThreadOnce();
  game_loop_ch.SetThreadId(GetGameLoopThreadId());
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
  CHECK(Config::Load(fs::canonical(fs::path(dll_path)).parent_path(),
                     RuntimeEnv::kGame));
  // Load tools.
  ToolHub::Init();
  ToolHub::instance()->LoadTools();
  LOG(INFO) << ToolHub::instance()->InspectMeta();
  // Create main window.
  HICON* phIcon = reinterpret_cast<HICON*>(0x00B7354C);
  HCURSOR* phCursor = reinterpret_cast<HCURSOR*>(0x00B73548);
  HWND* phWnd = reinterpret_cast<HWND*>(0x00B73550);
  HIMC* phImc = reinterpret_cast<HIMC*>(0x00B7355C);
  int* pnShowCmd = reinterpret_cast<int*>(0x00B73544);
  int* no_window_frame = reinterpret_cast<int*>(0x00BAC000);
  *phIcon = LoadIconA(hInstance, (LPCSTR)0x5D);
  *phCursor = LoadCursorA(hInstance, (LPCSTR)0x68);
  int screen_width = GetSystemMetrics(0);
  int screen_height = GetSystemMetrics(1);
  bool with_border =
      ((*no_window_frame) == 1 ||
       screen_height == yrpp::GameOptionsClass::Instance->ScreenHeight &&
       screen_width == yrpp::GameOptionsClass::Instance->ScreenWidth);
  gui::Window::Params params{
      .name = "Yuri's Revenge",
      .width = width,
      .height = height,
      .with_border = with_border,
      .hInstance = hInstance,
      .nShowCmd = nShowCmd,
      .wndproc = WndProc,
      .hIcon = *phIcon,
      .hCursor = *phCursor,
  };
  gui::Window::Init(params, /*post_init=*/[&](void* hwnd) {
    HWND hWnd = reinterpret_cast<HWND>(hwnd);
    *phWnd = hWnd;
    *phImc = ImmGetContext(hWnd);
    *pnShowCmd = nShowCmd;
    ImmAssociateContext(hWnd, *phImc);
    ShowWindow(hWnd, 1);
    UpdateWindow(hWnd);
    SetFocus(hWnd);
    RegisterHotKey(hWnd, 1, 7u, 0x4Du);
    HCURSOR cursor = LoadCursorA(params.hInstance, (LPCSTR)104);
    SetCursor(cursor);

    if (on_create_window) {
      on_create_window(hWnd);
    }
  });
}

static void __declspec(naked) __cdecl InjectCreateWindow() {
  static HINSTANCE hInstance;  // from WinMain
  static int nShowCmd;         // from WinMain
  static int width, height;
  __asm {
    mov hInstance, ecx
    mov nShowCmd, edx
    mov eax, [esp+4]
    mov width, eax
    mov eax, [esp+8]
    mov height, eax
    pushad
  }
  CreateWindowImpl(hInstance, width, height, nShowCmd);
  __asm {
    popad
    retn 8
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
  if (ToolHub::instance() != nullptr) {
    ToolHub::instance()->Update(delta_sec);
  }
  game_loop_ch.ExecuteTasks();
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
  auto ra2WndProc = reinterpret_cast<WNDPROC>(0x007775C0);
  CHECK_NOTNULL(ra2WndProc);
  HWND hWnd = *(reinterpret_cast<HWND*>(0x00B73550));
  CallWindowProc(ra2WndProc, hWnd, WM_DESTROY, NULL, NULL);
  DestroyWindowOnce();
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

TaskQueue& GetGameLoopChannel() { return game_loop_ch; }

void HookCreateWindow() {
  DLOG_F(INFO, "[SCARLET-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpCreateWindow, InjectCreateWindow);
}

void SetOnWindowCreated(std::function<void(HWND)> cb) {
  on_create_window = std::move(cb);
}

void DestroyWindowOnce() {
  static std::once_flag destroy_flag;
  std::call_once(destroy_flag, []() { DoDestroyWindow(); });
}

void HookUpdate() {
  DLOG_F(INFO, "[SCARLET-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpUpdate, InjectUpdate);
}

void HookExitGame() {
  DLOG_F(INFO, "[SCARLET-HOOK] {}", __func__);
  MemoryAPI::instance()->HookJump(kHpExitGame, InjectExitGame);
}

}  // namespace hook
}  // namespace yrtr
