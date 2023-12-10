cmake_minimum_required(VERSION 3.20)
project(DX_Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(DX_Test WIN32 directx_test.cpp d3dx12.h)
set_property(TARGET DX_Test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

include_directories(PUBLIC "C:\\Program Files (x86)\\DirectX")
include_directories(PUBLIC "C:\\Program Files (x86)\\skia")

add_subdirectory("DirectX-Headers")

# target_link_libraries(DX_Test user32 d3d12 comdlg32 dxgi dxguid uuid kernel32 shell32 ole32 oleaut32 dxgi DirectX-Headers DirectX-Guids)
target_link_libraries(DX_Test user32 d3d12 dxgi skia d3d12allocator d3dcompiler opengl32)
