cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wshadow")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-limit-debug-info -glldb")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(SOURCE_FILES src/graph/digraph.hpp src/graph/edge.hpp src/sim/sim.cpp)

include_directories(.)
add_executable(lexsim ${SOURCE_FILES})
