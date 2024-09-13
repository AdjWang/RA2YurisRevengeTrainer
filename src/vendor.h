#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <time.h>

#include <windows.h>

#include <filesystem>
#include <functional>
#include <format>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <span>
#include <sstream>
#include <thread>

#include <fmt/core.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include <toml++/toml.hpp>
