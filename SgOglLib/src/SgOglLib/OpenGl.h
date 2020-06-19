// This file is part of the SgOgl package.
// 
// Filename: OpenGl.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

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

//-------------------------------------------------
// OpenGL States
//-------------------------------------------------

namespace sg::ogl
{
    struct Color;

    class OpenGl
    {
    public:
        static void SetClearColor(const Color& t_color);
        static void Clear();
        static void ClearColorAndDepthBuffer();

        static void EnableDepthTesting();
        static void DisableDepthTesting();
        static void EnableDepthAndStencilTesting();

        static void EnableWritingIntoDepthBuffer();
        static void DisableWritingIntoDepthBuffer();

        static void EnableFaceCulling();
        static void DisableFaceCulling();

        static void EnableAlphaBlending();
        static void EnableAdditiveBlending();
        static void DisableBlending();

        static void EnableClipping(uint32_t t_nrOfClipDistances = 0);
        static void DisableClipping(uint32_t t_nrOfClipDistances = 0);

        static void EnableWireframeMode();
        static void DisableWireframeMode();

        static void SetDepthFunc(uint32_t t_func);

    protected:

    private:

    };
}
