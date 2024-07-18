#include "common/logging.h"
#include "trainer/gui.h"
#include "common/vendor.h"

namespace {
static void error_callback(int error, const char* description) {
    LOG(FATAL, "error=[{}] {}", error, description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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

    window = glfwCreateWindow(600, 900, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    glfwSetKeyCallback(window, key_callback);

    yrtr::ImGuiContext gui_ctx(window);

    int BtnClickCount = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
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
