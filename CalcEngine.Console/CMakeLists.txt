cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 17)
add_executable(CalcEngine.Console CalcEngine.Console.cpp)
include_directories(${CMAKE_SOURCE_DIR}/CalcEngine)
target_link_libraries(CalcEngine.Console CalcEngine)
include_directories(/mnt/c/Users/ericm/source/repos/vcpkg/vcpkg/installed/x86-windows/include/ /usr/include)

install(TARGETS CalcEngine.Console RUNTIME DESTINATION bin)