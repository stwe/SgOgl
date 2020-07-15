cmake_minimum_required(VERSION 3.16)

project(Sandbox)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SANDBOX_SRC_FILES
    "*.h"
    "*.cpp"
)

include(../conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} ${SANDBOX_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} SgOglLib)
