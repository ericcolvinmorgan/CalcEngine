cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES "*.cpp")
add_library(CalcEngine ${SOURCES})
include_directories(/mnt/c/Users/ericm/source/repos/vcpkg/vcpkg/installed/x86-windows/include/ /usr/include)

install(TARGETS CalcEngine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)