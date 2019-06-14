cmake_minimum_required(VERSION 3.8)

project(SgOgl)

set(CMAKE_CXX_STANDARD 17)

option(SHARED_LIB "Build shared lib" OFF)

add_subdirectory(Sandbox)
add_subdirectory(SgOglLib)
