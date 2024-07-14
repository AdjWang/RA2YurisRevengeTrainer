#include "vendor.h"
#include "logging.h"
#include "win32utils.h"

namespace yrtr {

ImGuiContext::ImGuiContext(GLFWwindow* window) {
    // setup high dpi scale factor
    ImGui_ImplWin32_EnableDpiAwareness();
    hdpi_scale_factor_ = ImGui_ImplWin32_GetDpiScaleForHwnd(GetDesktopWindow());
    LOG(INFO, "high dpi scale factor={:.2f}", hdpi_scale_factor_);
    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Allow popup windows outside of the main window
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        (win32::GetDefaultFontPath() / "arial.ttf").string().c_str(),
        18.0f * hdpi_scale_factor_);
    CHECK(font != nullptr);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::GetStyle().ScaleAllSizes(hdpi_scale_factor_);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // ImGui_ImplOpenGL3_Init("#version 460 core");
    ImGui_ImplOpenGL3_Init("#version 130");
}

ImGuiContext::~ImGuiContext() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void ImGuiContext::PreRender() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiContext::Render() const {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace yrtr

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    glfwSetKeyCallback(window, key_callback);

    yrtr::ImGuiContext gui_ctx(window);

    int BtnClickCount = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        gui_ctx.Update();
        gui_ctx.PreRender();

#ifdef IMGUI_HAS_VIEWPORT
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
#else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
        // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        bool show = true;
        ImGui::Begin("Status", &show,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::Button("Demo")) {
            BtnClickCount++;
        }
        ImGui::Text(
            std::format("Demo button clicked={}", BtnClickCount).c_str());
        ImGui::End();
        // ImGui::PopStyleVar(1);

        gui_ctx.Render();

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
