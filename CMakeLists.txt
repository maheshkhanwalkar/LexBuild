cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -stdlib=libc++")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(SOURCE_FILES
        Graph/WeightedDigraph.tpp
        FA/NFA.cpp FA/NFA.hpp FA/DFA.cpp FA/DFA.hpp
        Regex/NFAGen.cpp Regex/NFAGen.hpp)

set(TEST_DIGRAPH ${SOURCE_FILES} Test/DigraphTest.cpp)
set(TEST_NFA ${SOURCE_FILES} Test/NFATest.cpp)
set(TEST_REGEX ${SOURCE_FILES} Test/RegexTest.cpp)

include_directories(.)
add_library(lexbuild STATIC ${SOURCE_FILES})

add_executable(lextest-digraph ${TEST_DIGRAPH})
add_executable(lextest-nfa ${TEST_NFA})
add_executable(lextest-regex ${TEST_REGEX})

add_test(NAME lextest-digraph COMMAND lextest-digraph)
add_test(NAME lextest-nfa COMMAND lextest-nfa)
add_test(NAME lextest-regex COMMAND lextest-regex)
