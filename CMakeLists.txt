cmake_minimum_required(VERSION 3.28)
set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_DEBUG_POSTFIX "")

# suppress warning C4740:
# flow in or out of inline asm code suppresses global optimization
# add_compile_options(/MP /Gy /wd4740 /fp:fast)
# add_link_options(/OPT:REF)
add_compile_options(/MP)
add_compile_definitions(_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)

project(gui)

find_package(OpenGL REQUIRED)

# https://www.glfw.org/docs/latest/build_guide.html
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "disable glfw examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "disable glfw tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "disable glfw docs")
set(GLFW_INSTALL OFF CACHE BOOL "use glfw as a submodule")
add_subdirectory(deps/glfw)

set(IMGUI_INCLUDES deps/imgui
                   deps/imgui/backends)
set(IMGUI_SOURCES  deps/imgui/imgui.cpp
                   deps/imgui/imgui_demo.cpp
                   deps/imgui/imgui_draw.cpp
                   deps/imgui/imgui_tables.cpp
                   deps/imgui/imgui_widgets.cpp
                   deps/imgui/backends/imgui_impl_glfw.cpp
                   deps/imgui/backends/imgui_impl_opengl3.cpp
                   deps/imgui/backends/imgui_impl_win32.cpp)
add_library(libimgui STATIC ${IMGUI_SOURCES})
target_include_directories(libimgui 
    PUBLIC deps/glfw
    PUBLIC deps/glfw/include
    PUBLIC ${IMGUI_INCLUDES})

add_executable(${PROJECT_NAME} src/trainer/main.cpp
                               src/trainer/logging.cpp)
target_include_directories(${PROJECT_NAME} 
                    PRIVATE deps/glfw
                   PRIVATE deps/glfw/include
                   PRIVATE ${IMGUI_INCLUDES}
                                           PRIVATE src/trainer)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL
                                      PRIVATE libimgui
                                      PRIVATE glfw)
