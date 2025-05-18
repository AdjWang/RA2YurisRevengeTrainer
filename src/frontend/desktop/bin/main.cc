#include <cstdint>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/debugging/symbolize.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/logging.h"
#include "base/thread.h"
#include "frontend/desktop/char_table.h"
#include "frontend/desktop/config.h"
#include "frontend/desktop/glfw.h"
#include "frontend/desktop/gui.h"
#include "frontend/desktop/gui_context.h"
#include "frontend/desktop/timer.h"
#include "protocol/model.h"

using namespace yrtr;
using namespace yrtr::frontend;

void SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

namespace {
static void ErrorCallback(int error, const char* description) {
  LOG_F(FATAL, "error=[{}] {}", error, description);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto gui_ctx =
      reinterpret_cast<GuiContext*>(glfwGetWindowUserPointer(window));
  SetViewport(0, 0, width, height);
  gui_ctx->UpdateViewport(width, height);
}

static std::unordered_map<int, FnLabel> kHotkeyTable;

static void InitHotkey(HWND hWnd) {
  Config* config = Config::instance();
  DCHECK_NOTNULL(config);
  for (int label = 0; label < static_cast<int>(FnLabel::kCount); label++) {
    int key = config->GetHotkey(static_cast<FnLabel>(label));
    if (key != GLFW_KEY_UNKNOWN) {
      int scancode = glfwGetKeyScancode(key);
      int vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK);
      CHECK(vk > 0);
      if (!RegisterHotKey(hWnd, label, Config::kWin32HotKeyMod | MOD_NOREPEAT,
                          vk)) {
        config->DisableHotkeyGUI(key);
        char key_name[10];
        if (GetKeyNameText(scancode << 16, key_name, 10) > 0) {
          LOG_F(WARNING, "RegisterHotkey failed. Key name={}", key_name);
        } else {
          LOG_F(WARNING, "RegisterHotkey failed. Key num={}", vk);
        }
        continue;
      }
    }
    kHotkeyTable.emplace(key, static_cast<FnLabel>(label));
  }
}

static FnLabel GetHotkeyLabel(int key) {
  if (kHotkeyTable.contains(key)) {
    return kHotkeyTable[key];
  } else {
    return FnLabel::kInvalid;
  }
}

static WNDPROC glfw_wndproc = NULL;
static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam) {
  if (uMsg == WM_HOTKEY &&
      (LOWORD(lParam) & Config::kWin32HotKeyMod) > 0) {
    // LOG(INFO, "msg={} key={}", uMsg, HIWORD(lParam));
    Gui* gui = reinterpret_cast<Gui*>(GetProp(hWnd, "Gui"));
    CHECK(gui);
    gui->Trigger(GetHotkeyLabel(HIWORD(lParam)));
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return CallWindowProc(glfw_wndproc, hWnd, uMsg, wParam, lParam);
}

}  // namespace

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int) {
#ifdef YRTR_LOG_FILE
  if (atexit(DumpLog) != 0) {
    perror("Failed to register log dump function");
  }
#endif
  logging::InitLogging(logging::LogSink::kDbgView);
  SetupRendererThreadOnce();

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
  // Load configurations.
  // Search configuration file at the same directory with the dll.
  CHECK(Config::Load(fs::canonical(fs::path(exe_path)).parent_path()));
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

  ImGuiWindow gui_ctx(window);
  State state;
  Gui gui(state, Config::instance()->lang());
  {
    BOOL res = SetProp(hWnd, "GuiContext", static_cast<GuiContext*>(&gui_ctx));
    CHECK(res) << GetLastError();
  }
  {
    BOOL res = SetProp(hWnd, "Gui", &gui);
    CHECK(res) << GetLastError();
  }
  // Trainer::Init(config::GetGlobalConfig().exec_name, gui_ctx,
  //                     state);
  // Timer::SetTimer(Trainer::kTimerIdProcWatch, 1.0 /*second*/,
  //                       std::bind_front(&Trainer::OnProcWatchTimer,
  //                                       Trainer::instance()));
  // Timer::SetTimer(Trainer::kTimerIdTrainerFunc, 0.3 /*second*/,
  //                       std::bind_front(&Trainer::OnFuncScanTimer,
  //                                       Trainer::instance()));

  glfwSetWindowUserPointer(window, &gui_ctx);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(window)) {
    Timer::Update();

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
