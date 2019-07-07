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
    uint32_t t_source,
    uint32_t t_type,
    uint32_t t_id,
    uint32_t t_severity,
    int32_t t_length,
    const char* t_message,
    const void* t_userParam
);
