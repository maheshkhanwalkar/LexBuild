cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -O0 -stdlib=libc++ -fno-limit-debug-info")

set(SOURCE_FILES
        Graph/WeightedDigraph.tpp
        FA/NFA.cpp FA/NFA.hpp FA/DFA.cpp FA/DFA.hpp)

set(TEST_DIGRAPH ${SOURCE_FILES} Test/DigraphTest.cpp)
set(TEST_NFA ${SOURCE_FILES} Test/NFATest.cpp)

include_directories(.)
add_library(lexbuild STATIC ${SOURCE_FILES})

add_executable(lextest-digraph ${TEST_DIGRAPH})
add_executable(lextest-nfa ${TEST_NFA})

add_test(NAME lextest-digraph COMMAND lextest-digraph)
add_test(NAME lextest-nfa COMMAND lextest-nfa)

