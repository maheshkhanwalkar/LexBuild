cmake_minimum_required(VERSION 3.5)
project(LexBuild)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(SOURCE_FILES Test/Main.cpp
        Graph/WeightedDigraph.tpp)

include_directories(.)
add_executable(LexTest ${SOURCE_FILES})
