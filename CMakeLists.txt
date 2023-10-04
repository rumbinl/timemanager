cmake_minimum_required(VERSION 3.20)
project(Timeman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Timeman src/TM_Main.cpp)
set_property(TARGET Timeman PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

include_directories(PUBLIC include/)

if (MACOS)
	include_directories(PUBLIC /usr/local/include)
	include_directories(PUBLIC /usr/local/include/skia)
	include_directories(PUBLIC /usr/local/include/gl3w)
endif (MACOS)

if (WIN32)
	include_directories(PUBLIC "C:\\Program Files (x86)\\gl3w")
	include_directories(PUBLIC "C:\\Program Files (x86)\\SDL3\\include")
	include_directories(PUBLIC "C:\\Program Files (x86)\\skia")
endif (WIN32)

add_subdirectory(src)


