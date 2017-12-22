cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wdocumentation -stdlib=libc++")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-limit-debug-info -glldb")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(SOURCE_FILES
        Graph/Digraph.hpp Graph/Edge.hpp
        FA/NFA.cpp FA/NFA.hpp FA/DFA.cpp FA/DFA.hpp
        Gen/NFAGen.cpp Gen/NFAGen.hpp
        FA/Edge/NFAWeight.cpp FA/Edge/NFAWeight.hpp
        Lexer/Lexer.hpp Lexer/RuleInfo.hpp
        Regex/Regex.hpp Regex/Builder.cpp Regex/Builder.hpp)

set(TEST_DIGRAPH Graph/Digraph.hpp Test/DigraphTest.cpp)
set(TEST_NFA ${SOURCE_FILES} Test/NFATest.cpp)
set(TEST_REGEX ${SOURCE_FILES} Test/RegexTest.cpp)
set(TEST_BUILDER ${SOURCE_FILES} Test/BuilderTest.cpp)
set(TEST_LEXER ${SOURCE_FILES} Test/LexerTest.cpp)

include_directories(.)
add_library(lexbuild STATIC ${SOURCE_FILES})

add_executable(lextest-digraph ${TEST_DIGRAPH})
add_executable(lextest-nfa ${TEST_NFA})
add_executable(lextest-regex ${TEST_REGEX})
add_executable(lextest-builder ${TEST_BUILDER})
add_executable(lextest-lexer ${TEST_LEXER})

add_test(NAME lextest-digraph COMMAND lextest-digraph)
add_test(NAME lextest-nfa COMMAND lextest-nfa)
add_test(NAME lextest-regex COMMAND lextest-regex)
add_test(NAME lextest-builder COMMAND lextest-builder)
add_test(NAME lextest-lexer COMMAND lextest-lexer)
