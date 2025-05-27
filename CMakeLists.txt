cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ra2_trainer)

option(YRTR_BUILD_TEST "Build unit tests." ON)
option(YRTR_MSVC_STATIC_RUNTIME "Link static runtime libraries" ON)
option(YRTR_FORCE_DCHECK "Force enable debug check." OFF)

set(YRTR_DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(YRTR_DEPS_OUTPUT_DIR ${YRTR_DEPS_DIR}/out/${CMAKE_BUILD_TYPE})
set(YRTR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(YRTR_MSVC_STATIC_RUNTIME)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Abseil log reports: warning C4127: conditional expression is constant
add_compile_options(/Zc:__cplusplus /MP /utf-8 /EHsc $<$<CONFIG:Release>:/Gy> /wd4127)
add_compile_definitions(
  NOMINMAX  # Disable min/max macros in minwindef.h. They are sucks!
  _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS
  $<$<CONFIG:Debug>:YRTR_DEBUG>   # facilitates searching
)
add_link_options($<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/INCREMENTAL:NO>)

# Add dependencies.
find_package(absl REQUIRED)
find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(plutosvg REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(websocketpp REQUIRED)

set(IMGUI_PROJ_DIR ${YRTR_DEPS_DIR}/imgui-1.91.9b-docking)
set(IMGUI_INCLUDES ${IMGUI_PROJ_DIR}
                   ${IMGUI_PROJ_DIR}/backends)
set(IMGUI_SOURCES  ${IMGUI_PROJ_DIR}/imgui.cpp
                   ${IMGUI_PROJ_DIR}/imgui_demo.cpp
                   ${IMGUI_PROJ_DIR}/imgui_draw.cpp
                   ${IMGUI_PROJ_DIR}/imgui_tables.cpp
                   ${IMGUI_PROJ_DIR}/imgui_widgets.cpp
                   ${IMGUI_PROJ_DIR}/misc/freetype/imgui_freetype.cpp
                   ${IMGUI_PROJ_DIR}/backends/imgui_impl_glfw.cpp
                   ${IMGUI_PROJ_DIR}/backends/imgui_impl_opengl3.cpp
                   ${IMGUI_PROJ_DIR}/backends/imgui_impl_win32.cpp)
add_library(libimgui STATIC ${IMGUI_SOURCES})
target_include_directories(libimgui PUBLIC ${IMGUI_INCLUDES})
target_compile_definitions(libimgui PUBLIC
  IMGUI_DISABLE_DEFAULT_ALLOCATORS   # use custom malloc/free across dlls
  IMGUI_USE_WCHAR32 IMGUI_ENABLE_FREETYPE IMGUI_ENABLE_FREETYPE_PLUTOSVG   # support emoji
)
target_link_libraries(libimgui PRIVATE Freetype::Freetype glfw plutosvg::plutosvg)

add_library(wsock32 SHARED
  ${YRTR_SOURCE_DIR}/wsock32/export_table.def
  ${YRTR_SOURCE_DIR}/wsock32/wsock32.cc
  ${YRTR_SOURCE_DIR}/base/logging.cc
)
target_include_directories(wsock32 PUBLIC ${YRTR_SOURCE_DIR})
target_compile_definitions(wsock32 PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_options(wsock32 PRIVATE /W4 $<$<CONFIG:Debug>:/WX>)
target_link_libraries(wsock32
  PRIVATE absl::log
  PRIVATE mswsock
  PRIVATE ws2_32
)

add_library(yrtr_base STATIC
  ${YRTR_SOURCE_DIR}/base/debug.cc
  ${YRTR_SOURCE_DIR}/base/logging.cc
  ${YRTR_SOURCE_DIR}/base/task_queue.cc
  ${YRTR_SOURCE_DIR}/base/thread.cc
)
target_include_directories(yrtr_base
  PUBLIC $<BUILD_INTERFACE:${YRTR_DEPS_OUTPUT_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${YRTR_SOURCE_DIR}>
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(yrtr_base
  PUBLIC absl::check
  PUBLIC absl::flat_hash_map
  PUBLIC absl::flat_hash_set
  PUBLIC absl::inlined_vector
  PUBLIC absl::log
  PUBLIC absl::synchronization
)

add_library(yrtr_frontend STATIC
  ${YRTR_SOURCE_DIR}/frontend/desktop/config.cc
  ${YRTR_SOURCE_DIR}/frontend/desktop/timer.cc
  ${YRTR_SOURCE_DIR}/frontend/desktop/gui.cc
  ${YRTR_SOURCE_DIR}/frontend/desktop/gui_context.cc
  ${YRTR_SOURCE_DIR}/protocol/client.cc
)
target_include_directories(yrtr_frontend
  PUBLIC $<BUILD_INTERFACE:${YRTR_DEPS_OUTPUT_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${YRTR_SOURCE_DIR}>
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(yrtr_frontend
  PRIVATE $<$<CONFIG:Debug>:YRTR_GL_DEBUG_CTX>
)
target_compile_options(yrtr_frontend PRIVATE /W4 $<$<CONFIG:Debug>:/WX>)
target_link_libraries(yrtr_frontend
  PRIVATE yrtr_base
  PRIVATE glfw
  PRIVATE libimgui
  PRIVATE opengl32
  PRIVATE tomlplusplus::tomlplusplus
)

add_executable(ra2_trainer WIN32 ${YRTR_SOURCE_DIR}/frontend/desktop/bin/main.cc)
target_compile_options(ra2_trainer PRIVATE /W4 $<$<CONFIG:Debug>:/WX>)
target_link_libraries(ra2_trainer PRIVATE yrtr_frontend glfw)

# TODO: not working
add_custom_command(TARGET ra2_trainer POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/scripts/ra2_trainer.toml
  ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

add_library(yrtr_backend STATIC
  ${YRTR_SOURCE_DIR}/backend/config.cc
  ${YRTR_SOURCE_DIR}/backend/game/beacon.cc
  ${YRTR_SOURCE_DIR}/backend/hook/game_loop.cc
  ${YRTR_SOURCE_DIR}/backend/hook/hook.cc
  ${YRTR_SOURCE_DIR}/backend/hook/logging.cc
  ${YRTR_SOURCE_DIR}/backend/hook/memory_api.cc
  ${YRTR_SOURCE_DIR}/backend/hook/trainer.cc
  ${YRTR_SOURCE_DIR}/protocol/server.cc
)
target_include_directories(yrtr_backend
  PUBLIC $<BUILD_INTERFACE:${YRTR_DEPS_OUTPUT_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${YRTR_DEPS_OUTPUT_DIR}/include/boost>
  PUBLIC $<BUILD_INTERFACE:${YRTR_DEPS_OUTPUT_DIR}/include/YRpp>
  PUBLIC $<BUILD_INTERFACE:${YRTR_SOURCE_DIR}>
  PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# warning C4740: flow in or out of inline asm code suppresses global optimization
target_compile_options(yrtr_backend PRIVATE /W4 $<$<CONFIG:Debug>:/WX> /wd4740)
target_link_libraries(yrtr_backend
  PRIVATE yrtr_base
  PRIVATE tomlplusplus::tomlplusplus
  PRIVATE comctl32
  PRIVATE winmm
)

add_library(ra2_trainer_backend SHARED ${YRTR_SOURCE_DIR}/backend/lib/dllmain.cc)
target_compile_options(ra2_trainer_backend PRIVATE /W4 $<$<CONFIG:Debug>:/WX>)
target_link_libraries(ra2_trainer_backend PRIVATE yrtr_backend)

if (YRTR_BUILD_TEST)
  add_executable(test_server
    ${YRTR_SOURCE_DIR}/backend/bin/test_server.cc
    ${YRTR_SOURCE_DIR}/backend/hook/mock_trainer.cc
  )
  target_link_libraries(test_server
    PRIVATE yrtr_base
    PRIVATE yrtr_backend
    PRIVATE absl::failure_signal_handler
  )

  add_executable(test_json ${YRTR_SOURCE_DIR}/protocol/test_json.cc)
  target_link_libraries(test_json PRIVATE yrtr_base)
endif()
