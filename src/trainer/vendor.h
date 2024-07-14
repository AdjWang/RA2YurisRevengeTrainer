#pragma once

#include <iostream>
#include <format>
#include <sstream>

#include <errno.h>
#include <filesystem>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

namespace yrtr {

class ImGuiContext {
public:
    ImGuiContext(GLFWwindow* window);
    ~ImGuiContext();

    void Update() {}
    void PreRender();
    void Render() const;

private:
    float hdpi_scale_factor_;
};

}  // namespace yrtr
