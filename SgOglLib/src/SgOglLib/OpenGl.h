#pragma once

// include Windows.h before glfw3.h
#ifdef _WIN64
    #include <Windows.h>
#endif

//-------------------------------------------------
// Stb Image
//-------------------------------------------------

#include "resource/stb_image.h"

//-------------------------------------------------
// Glew
//-------------------------------------------------

#ifdef _WIN64
    #define GLEW_STATIC
#endif
#include <GL/glew.h>

//-------------------------------------------------
// GLFW3
//-------------------------------------------------

#include <GLFW/glfw3.h>

//-------------------------------------------------
// Debug output
//-------------------------------------------------

void APIENTRY gl_debug_output(
    GLenum t_source,
    GLenum t_type,
    GLuint t_id,
    GLenum t_severity,
    GLsizei t_length,
    const GLchar* t_message,
    const void* t_userParam
);
