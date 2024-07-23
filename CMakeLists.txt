cmake_minimum_required(VERSION 3.28)
set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_DEBUG_POSTFIX "")

add_compile_options(/MP /Gy /GS-)
add_compile_definitions(_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
add_link_options(/OPT:REF /INCREMENTAL:NO /level='requireAdministrator' /uiAccess='false')

# add_compile_definitions(YRTR_LOG_STD)
add_compile_definitions(YRTR_LOG_DBGVIEW)

project(ra2_trainer)

find_package(OpenGL REQUIRED)

set(FMT_DOC OFF CACHE BOOL "disable fmt doc")
set(FMT_TEST OFF CACHE BOOL "disable fmt test")
set(FMT_INSTALL OFF CACHE BOOL "disable fmt install target")
add_subdirectory(deps/fmt)

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

add_subdirectory(src/aa_engine)

add_executable(${PROJECT_NAME} WIN32 src/bin/main.cpp
                                     src/logging.cpp
                                     src/timer.cpp
                                     src/gui.cpp
                                     src/win32utils.cpp
                                     src/trainer.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE deps/fmt/include
                                           PRIVATE deps/glfw
                                           PRIVATE deps/glfw/include
                                           PRIVATE ${IMGUI_INCLUDES}
                                           PRIVATE src)
target_compile_options(${PROJECT_NAME} PRIVATE /W4
                                       PRIVATE /WX
                                       PRIVATE /utf-8)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL
                                      PRIVATE libimgui
                                      PRIVATE glfw
                                      PRIVATE fmt::fmt
                                      PRIVATE aa_engine)

add_executable(disasm src/bin/test_disasm.cpp
                      src/logging.cpp
                      src/win32utils.cpp)
target_include_directories(disasm PRIVATE deps/fmt/include
                                  PRIVATE deps/glfw
                                  PRIVATE deps/glfw/include
                                  PRIVATE ${IMGUI_INCLUDES}
                                  PRIVATE src)
target_link_libraries(disasm PRIVATE fmt::fmt
                             PRIVATE aa_engine)
