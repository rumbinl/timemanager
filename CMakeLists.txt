cmake_minimum_required(VERSION 3.20)
project(Timeman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Timeman src/TM_Main.cpp)

include_directories(PUBLIC include/)
include_directories(PUBLIC /usr/local/include)
include_directories(PUBLIC /usr/local/include/skia)
include_directories(PUBLIC /usr/local/include/gl3w)

add_subdirectory(src)


