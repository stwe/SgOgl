// This file is part of the SgOgl package.
// 
// Filename: Vbo.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>
#include "OpenGl.h"

namespace sg::ogl::buffer
{
    class Vbo
    {
    public:
        //-------------------------------------------------
        // Live and let die
        //-------------------------------------------------

        static uint32_t GenerateVbo();
        static uint32_t GenerateEbo();
        static void DeleteVbo(uint32_t t_vboId);
        static void DeleteEbo(uint32_t t_eboId);

        //-------------------------------------------------
        // Bind / Unbind
        //-------------------------------------------------

        static void BindVbo(uint32_t t_vboId, uint32_t t_target = GL_ARRAY_BUFFER);
        static void BindEbo(uint32_t t_eboId);
        static void UnbindVbo();

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        static void InitEmpty(uint32_t t_vboId, uint32_t t_floatCount, uint32_t t_usage = GL_STREAM_DRAW);

        //-------------------------------------------------
        // Store data
        //-------------------------------------------------

        static void StoreTransformationMatrices(uint32_t t_vboId, uint32_t t_floatCount, const std::vector<glm::mat4>& t_matrices);

        //-------------------------------------------------
        // Attributes
        //-------------------------------------------------

        /**
         * @brief Function to define per instance data.
         * @param t_vboId The Vbo for which the attribute is to be defined.
         * @param t_index The index of the vertex attribute.
         * @param t_dataSize The number of components for this attribute. Must be 1, 2, 3, or 4.
         * @param t_instancedDataLength Amount of bytes between each instance.
         * @param t_offset Pointer to the starting point of this attribute.
         */
        static void AddInstancedAttribute(uint32_t t_vboId, uint32_t t_index, int32_t t_dataSize, int32_t t_instancedDataLength, uint64_t t_offset);

    protected:

    private:

    };
}
