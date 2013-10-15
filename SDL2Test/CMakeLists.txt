cmake_minimum_required(VERSION 2.8.10)
project(SDL2Test)

#include(MacroEnsureOutOfSourceBuild)
#macro_ensure_out_of_source_build("SDL2Test requires an out of source build. Please create a separate build directory and run 'cmake /path/to/SDL2TEST [options]' there.")

set(CMAKE_CXX_FLAGS "-g -Wall")

file(GLOB_RECURSE SDL2Test_SRC "src/*.cpp")
file(GLOB_RECURSE SDL2TEST_INC "include/*.h")

set(SDL2Test_INCLUDE_DIRS "include")

include_directories(${SDL2Test_INCLUDE_DIRS})
link_directories(/usr/local/lib)
add_executable(SDL2TestDebug ${SDL2Test_SRC})
target_link_libraries(SDL2TestDebug SDL2)
