#include "logging.h"
#include "gui.h"
#include "vendor.h"
#include "trainer_func.h"

namespace {
static void error_callback(int error, const char* description) {
    LOG(FATAL, "error=[{}] {}", error, description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static constexpr double kTimerInterval = 1.0;   // second
static void OnTimer(yrtr::ImGuiContext& ctx) {
    static int count = 0;
    ctx.set_state(std::format("nice! {}", count++));
}
}  // namespace

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(556, 900, "RA2 Trainer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    glfwSetKeyCallback(window, key_callback);

    yrtr::ImGuiContext gui_ctx(window);
    // Disable imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    // Init gui components
    yrtr::InitGUI(gui_ctx);

    int BtnClickCount = 0;
    double ts = glfwGetTime();
    double last_ts = ts;

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
        ts = glfwGetTime();
        if (ts - last_ts > kTimerInterval) {
            last_ts = ts;
            OnTimer(gui_ctx);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gui_ctx.Update();
        gui_ctx.PreRender();
        gui_ctx.RenderClientArea();
        gui_ctx.Render();

        glfwSwapBuffers(window);
        glfwWaitEventsTimeout(1.0);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
