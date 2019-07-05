project(SgOglLib)

set(LIB_CORE_SRC_FILES
    src/SgOglLib/Application.cpp
    src/SgOglLib/Color.cpp
    src/SgOglLib/Config.cpp
    src/SgOglLib/Log.cpp
    src/SgOglLib/OpenGl.cpp
    src/SgOglLib/SgOglException.cpp
    src/SgOglLib/Window.cpp
)

set(LIB_BUFFER_SRC_FILES
    src/SgOglLib/buffer/BufferLayout.cpp
    src/SgOglLib/buffer/Vao.cpp
)

set(LIB_CAMERA_SRC_FILES
    src/SgOglLib/camera/LookAtCamera.cpp
)

set(LIB_EVENT_SRC_FILES
    src/SgOglLib/event/CircularEventQueue.cpp
)

set(LIB_INPUT_SRC_FILES
    src/SgOglLib/input/MouseInput.cpp
)

set(LIB_RES_SRC_FILES
    src/SgOglLib/resource/ShaderManager.cpp
    src/SgOglLib/resource/ShaderProgram.cpp
    src/SgOglLib/resource/StbImage.cpp
    src/SgOglLib/resource/TextureManager.cpp
)

set(LIB_STATE_SRC_FILES
    src/SgOglLib/state/State.cpp
    src/SgOglLib/state/StateStack.cpp
)

set(LIB_SRC_FILES
    ${LIB_CORE_SRC_FILES}
    ${LIB_BUFFER_SRC_FILES}
    ${LIB_CAMERA_SRC_FILES}
    ${LIB_EVENT_SRC_FILES}
    ${LIB_INPUT_SRC_FILES}
    ${LIB_RES_SRC_FILES}
    ${LIB_STATE_SRC_FILES}
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

# include dirs
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/SgOglLib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/tinyxml2/include)

# add tinyxml2 lib
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/tinyxml2/lib/static/linux/libtinyxml2.a)

# add glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# add glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GLEW_LIBRARY} )

set(OpenGL_GL_PREFERENCE "LEGACY")

# add OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY})

# set an alias for this lib
add_library(Sg::GameEngineLib ALIAS ${PROJECT_NAME})
