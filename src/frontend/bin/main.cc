#include <cstdint>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "absl/debugging/symbolize.h"
#include "base/logging.h"
#include "base/thread.h"
#include "frontend/char_table.h"
#include "frontend/config.h"
#include "frontend/glfw.h"
#include "frontend/gui.h"
#include "frontend/gui_context.h"
#include "frontend/timer.h"
#include "protocol/model.h"

void SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

namespace {
static void ErrorCallback(int error, const char* description) {
  LOG_F(FATAL, "error=[{}] {}", error, description);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto gui_ctx =
      reinterpret_cast<yrtr::GuiContext*>(glfwGetWindowUserPointer(window));
  SetViewport(0, 0, width, height);
  gui_ctx->UpdateViewport(width, height);
}

static std::unordered_map<int, yrtr::FnLabel> kHotkeyTable;

static void InitHotkey(HWND hWnd) {
  // TODO
  UNREFERENCED_PARAMETER(hWnd);
  // for (int label = 0; label < (int)yrtr::FnLabel::kCount; label++) {
  //   int key = yrtr::config::GetHotkey((yrtr::FnLabel)label);
  //   if (key != GLFW_KEY_UNKNOWN) {
  //     int scancode = glfwGetKeyScancode(key);
  //     int vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK);
  //     CHECK(vk > 0);
  //     if (!RegisterHotKey(hWnd, label,
  //                         yrtr::config::kWin32HotKeyMod | MOD_NOREPEAT, vk)) {
  //       yrtr::DisableHotkeyGUI(key);
  //       char key_name[10];
  //       if (GetKeyNameText(scancode << 16, key_name, 10) > 0) {
  //         LOG(WARN, "RegisterHotkey failed. Key name={}", key_name);
  //       } else {
  //         LOG(WARN, "RegisterHotkey failed. Key num={}", vk);
  //       }
  //       continue;
  //     }
  //   }
  //   kHotkeyTable.emplace(yrtr::config::GetHotkey((yrtr::FnLabel)label),
  //                        (yrtr::FnLabel)label);
  // }
}

// static yrtr::FnLabel GetHotkeyLabel(int key) {
//   if (kHotkeyTable.contains(key)) {
//     return kHotkeyTable[key];
//   } else {
//     return yrtr::FnLabel::kInvalid;
//   }
// }

static WNDPROC glfw_wndproc = NULL;
static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam) {
  // TODO
  // if (uMsg == WM_HOTKEY &&
  //     (LOWORD(lParam) & yrtr::config::kWin32HotKeyMod) > 0) {
  //   // LOG(INFO, "msg={} key={}", uMsg, HIWORD(lParam));
  //   yrtr::Gui* gui = reinterpret_cast<yrtr::Gui*>(GetProp(hWnd, "Gui"));
  //   CHECK(gui);
  //   gui->Trigger(GetHotkeyLabel(HIWORD(lParam)));
  //   return DefWindowProc(hWnd, uMsg, wParam, lParam);
  // }
  return CallWindowProc(glfw_wndproc, hWnd, uMsg, wParam, lParam);
}

}  // namespace

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int) {
#ifdef YRTR_LOG_FILE
  if (atexit(yrtr::DumpLog) != 0) {
    perror("Failed to register log dump function");
  }
#endif
  yrtr::logging::InitLogging(yrtr::logging::LogSink::kDbgView);
  yrtr::SetupRendererThreadOnce();

  char exe_path[MAX_PATH] = {0};
  DWORD nSize = GetModuleFileNameA(hInstance, exe_path, _countof(exe_path));
  if (nSize == 0) {
    DWORD err = GetLastError();
    std::string message = std::system_category().message(err);
    LOG_F(FATAL, "Failed to get module file name, err=[{}]{}", err, message);
    UNREACHABLE();
  }
  absl::InitializeSymbolizer(exe_path);

  GLFWwindow* window;
  glfwSetErrorCallback(ErrorCallback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(556, 900, "RA2 Trainer", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // GLFW not implements global hotkey listener, thus the win32 message hook
  // is necessary
  HWND hWnd = glfwGetWin32Window(window);
  InitHotkey(hWnd);
  glfw_wndproc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
  CHECK(glfw_wndproc) << GetLastError();
  LRESULT res1 = SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
  CHECK(res1) << GetLastError();

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  // Load configurations.
  // Search configuration file at the same directory with the dll.
  CHECK(yrtr::Config::Load(fs::canonical(fs::path(exe_path)).parent_path()));
  yrtr::ImGuiWindow gui_ctx(window);
  yrtr::State state;
  yrtr::Gui gui(state, yrtr::Config::instance()->lang());
  {
    BOOL res =
        SetProp(hWnd, "GuiContext", static_cast<yrtr::GuiContext*>(&gui_ctx));
    CHECK(res) << GetLastError();
  }
  {
    BOOL res = SetProp(hWnd, "Gui", &gui);
    CHECK(res) << GetLastError();
  }
  // yrtr::Trainer::Init(yrtr::config::GetGlobalConfig().exec_name, gui_ctx,
  //                     state);
  // yrtr::Timer::SetTimer(yrtr::Trainer::kTimerIdProcWatch, 1.0 /*second*/,
  //                       std::bind_front(&yrtr::Trainer::OnProcWatchTimer,
  //                                       yrtr::Trainer::instance()));
  // yrtr::Timer::SetTimer(yrtr::Trainer::kTimerIdTrainerFunc, 0.3 /*second*/,
  //                       std::bind_front(&yrtr::Trainer::OnFuncScanTimer,
  //                                       yrtr::Trainer::instance()));

  glfwSetWindowUserPointer(window, &gui_ctx);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(window)) {
    yrtr::Timer::Update();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    gui_ctx.BeginFrame();
    gui.Render();
    gui_ctx.EndFrame();
    gui_ctx.Render();

    glfwSwapBuffers(window);
    glfwWaitEventsTimeout(0.2);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
