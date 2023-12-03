cmake_minimum_required(VERSION 3.25)
project(3Dcube)

set(CMAKE_CXX_STANDARD 23)

add_executable(3Dcube main.cpp renderer3d.h renderer3d.cpp)


include_directories(C:/Users/Dandy/vcpkg/installed/x64-mingw-dynamic/include)
include_directories(C:/Users/Dandy/vcpkg/installed/x64-mingw-static/include)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})