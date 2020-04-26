cmake_minimum_required(VERSION 3.16)

project(SgOglLib)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE LIB_SRC_FILES
    "*.h"
    "*.cpp"
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${PROJECT_NAME} STATIC ${LIB_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/SgOglLib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/gli)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("-- USE STATIC DEBUG SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC SG_OGL_DEBUG_BUILD GLFW_INCLUDE_NONE)
else()
    message("-- USE STATIC RELEASE SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
endif()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
