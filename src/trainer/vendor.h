#pragma once

#include <windows.h>
#include <iostream>
#include <format>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

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
