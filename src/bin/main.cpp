#include "config.h"
#include "logging.h"
#include "gui.h"
#include "vendor.h"
#include "trainer.h"
#include "char_table.h"
#include "timer.h"
#include "state.h"

namespace {
static void ErrorCallback(int error, const char* description) {
    LOG(FATAL, "error=[{}] {}", error, description);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto gui_ctx =
        reinterpret_cast<yrtr::GuiContext*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    gui_ctx->UpdateViewport(window, width, height);
}

static std::unordered_map<int, yrtr::FnLabel> kHotkeyTable;

static void InitHotkey(HWND hWnd) {
    for (int label = 0; label < (int)yrtr::FnLabel::kCount; label++) {
        kHotkeyTable.emplace(yrtr::config::GetHotkey((yrtr::FnLabel)label),
                             (yrtr::FnLabel)label);
        int key = yrtr::config::GetHotkey((yrtr::FnLabel)label);
        if (key != GLFW_KEY_UNKNOWN) {
            int scancode = glfwGetKeyScancode(key);
            int vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK);
            CHECK(vk > 0);
            CHECK(RegisterHotKey(
                hWnd, label, yrtr::config::kWin32HotKeyMod | MOD_NOREPEAT, vk));
        }
    }
}

static yrtr::FnLabel GetHotkeyLabel(int key) {
    if (kHotkeyTable.contains(key)) {
        return kHotkeyTable[key];
    } else {
        return yrtr::FnLabel::kInvalid;
    }
}

static WNDPROC glfw_wndproc = NULL;
static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                              LPARAM lParam) {
    if (uMsg == WM_HOTKEY &&
        (LOWORD(lParam) & yrtr::config::kWin32HotKeyMod) > 0) {

        LOG(INFO, "msg={} key={}", uMsg, HIWORD(lParam));
        yrtr::GuiContext* gui_ctx =
            (yrtr::GuiContext*)GetProp(hWnd, "GuiContext");
        CHECK(gui_ctx);
        gui_ctx->Trigger(GetHotkeyLabel(HIWORD(lParam)));
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return CallWindowProc(glfw_wndproc, hWnd, uMsg, wParam, lParam);
}

}  // namespace

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
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

    yrtr::State state;
    yrtr::GuiContext gui_ctx(window, state);
    BOOL res2 = SetProp(hWnd, "GuiContext", &gui_ctx);
    CHECK(res2) << GetLastError();
    yrtr::Trainer::Init("gamemd.exe", gui_ctx, state);
    yrtr::Timer::SetTimer(yrtr::Trainer::kTimerIdProcWatch, 1.0 /*second*/,
                          std::bind_front(&yrtr::Trainer::OnProcWatchTimer,
                                          yrtr::Trainer::instance()));
    yrtr::Timer::SetTimer(yrtr::Trainer::kTimerIdTrainerFunc, 0.3 /*second*/,
                          std::bind_front(&yrtr::Trainer::OnFuncScanTimer,
                                          yrtr::Trainer::instance()));

    glfwSetWindowUserPointer(window, &gui_ctx);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Disable imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
        yrtr::Timer::Update();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gui_ctx.Update();
        gui_ctx.PreRender();
        gui_ctx.RenderClientArea();
        gui_ctx.Render();

        glfwSwapBuffers(window);
        glfwWaitEventsTimeout(0.2);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
