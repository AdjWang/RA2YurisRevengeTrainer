cmake_minimum_required(VERSION 3.28)
set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/MP /Gy /J)
add_link_options(/OPT:REF /INCREMENTAL:NO)

project(aa_engine)

add_library(${PROJECT_NAME} STATIC disasm.c
                                   asmserv.c
                                   assembl.c
                                   tools.cpp
                                   aobscan.cpp
                                   autoassembler.cpp)
