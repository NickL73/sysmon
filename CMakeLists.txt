cmake_minimum_required(VERSION 3.25)
project(system-monitor VERSION 1.0.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_compile_options(-Wall -Wextra -Wconversion -Wunreachable-code -Wstrict-prototypes -Wpointer-arith -pedantic)

add_subdirectory(remote-src)
