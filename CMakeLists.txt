cmake_minimum_required(VERSION 3.5)
project(LexBuild)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-limit-debug-info -glldb")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# set(SOURCE_FILES )

include_directories(.)
add_library(lexbuild STATIC ${SOURCE_FILES})
