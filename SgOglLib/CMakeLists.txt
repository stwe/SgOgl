project(SgOglLib)

set(LIB_SRC_FILES
    src/SgOglLib/Application.cpp
    src/SgOglLib/Log.cpp
)

if (SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED ${LIB_SRC_FILES})
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        message("-- USE SHARED DEBUG SETUP")
        target_compile_definitions(${PROJECT_NAME} PUBLIC SG_OGL_DLL SG_OGL_DLL_EXPORTS SG_OGL_DEBUG_BUILD)
    else()
        message("-- USE SHARED RELEASE SETUP")
        target_compile_definitions(${PROJECT_NAME} PUBLIC SG_OGL_DLL SG_OGL_DLL_EXPORTS)
    endif()
else()
    add_library(${PROJECT_NAME} STATIC ${LIB_SRC_FILES})
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        message("-- USE STATIC DEBUG SETUP")
        target_compile_definitions(${PROJECT_NAME} PUBLIC SG_OGL_DEBUG_BUILD)
    else()
        message("-- USE STATIC RELEASE SETUP")
    endif()
endif()

add_library(Sg::GameEngineLib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/SgOglLib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
