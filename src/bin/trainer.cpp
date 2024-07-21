#include "logging.h"
#include "gui.h"
#include "vendor.h"
#include "trainer_func.h"
#include "char_table.h"

namespace {
static void error_callback(int error, const char* description) {
    LOG(FATAL, "error=[{}] {}", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    UNREFERENCED_PARAMETER(scancode);
    UNREFERENCED_PARAMETER(mods);
    // TODO
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto gui_ctx =
        reinterpret_cast<yrtr::GuiContext*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    gui_ctx->UpdateViewport(window, width, height);
}

static constexpr double kTimerInterval = 1.0;   // second
static void OnTimer(yrtr::GuiContext& ctx) {
    yrtr::TrainerFunc::instance()->Update();

    if (yrtr::TrainerFunc::instance()->attached()) {
        ctx.set_state(yrtr::GetFnChar(yrtr::FnLabel::kStateOk));
    } else {
        ctx.set_state(yrtr::GetFnChar(yrtr::FnLabel::kStateIdle));
    }
}
}  // namespace

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
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

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync
    yrtr::GuiContext gui_ctx(window);
    glfwSetWindowUserPointer(window, &gui_ctx);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Disable imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    yrtr::TrainerFunc::Init("gamemd.exe", gui_ctx);

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
