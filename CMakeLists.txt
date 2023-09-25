cmake_minimum_required(VERSION 3.10)

project(Timeman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Timeman src/main.cpp)

include_directories(/usr/local/include/gl3w /usr/local/include/skia /usr/local/include)

target_include_directories(Timeman PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB files "src/*.cpp")
foreach(file ${files})
    get_filename_component(libname ${file} NAME_WE)
    add_library(${libname} ${file})
endforeach()