cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

# Top-level CXX flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wdocumentation")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(.)

# Project subdirectories
add_subdirectory(LexBuild)
