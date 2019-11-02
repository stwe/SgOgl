// This file is part of the SgOgl package.
// 
// Filename: Vao.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>
#include <vector>
#include "OpenGl.h"

namespace sg::ogl::buffer
{
    class BufferLayout;

    /**
     * @brief The Vao class wraps an OpenGL Vertex Array Object (Vao).
     *        A Vao is an object which contains one or more Vertex Buffer Objects (Vbo).
     */
    class Vao
    {
    public:
        using VboContainer = std::vector<uint32_t>;
        using IndexContainer = std::vector<uint32_t>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Vao();

        Vao(const Vao& t_other) = delete;
        Vao(Vao&& t_other) noexcept = delete;
        Vao& operator=(const Vao& t_other) = delete;
        Vao& operator=(Vao&& t_other) noexcept = delete;

        ~Vao() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetVaoId() const;
        const VboContainer& GetVbos() const;
        uint32_t GetEboId() const;
        bool HasIndexBuffer() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetDrawCount(int32_t t_drawCount);

        //-------------------------------------------------
        // Vao
        //-------------------------------------------------

        void BindVao() const;
        static void UnbindVao();

        //-------------------------------------------------
        // Vbos
        //-------------------------------------------------

        uint32_t GenerateVbo();
        void BindVbo(uint32_t t_id) const;
        static void UnbindVbo();

        /**
         * @brief Add an empty (nullptr) Vertex Buffer Object (Vbo).
         * @param t_floatCount The maximum number of floats.
         * @param t_usage Specifies the expected usage pattern of the data store.
         * @return The id of the new created Vbo.
         */
        uint32_t AddEmptyVbo(uint32_t t_floatCount, uint32_t t_usage = GL_STREAM_DRAW);

        /**
         * @brief Add a Vertex Buffer Object (Vbo) an copy the given vertex data to the Gpu.
         *        The function sets draw count.
         * @param t_vertices Pointer to vertex data.
         * @param t_drawCount The number of draw count.
         * @param t_bufferLayout Metadata to interpret the vertex data.
         */
        void AddVertexDataVbo(float* t_vertices, int32_t t_drawCount, const BufferLayout& t_bufferLayout);

        /**
         * @brief Function to define per instance data.
         * @param t_vboId The Vbo for which the attribute is to be defined.
         * @param t_index The index of the vertex attribute.
         * @param t_dataSize The number of components for this attribute. Must be 1, 2, 3, or 4.
         * @param t_instancedDataLength Amount of bytes between each instance.
         * @param t_offset Pointer to the starting point of this attribute.
         */
        void AddInstancedAttribute(uint32_t t_vboId, uint32_t t_index, int32_t t_dataSize, int32_t t_instancedDataLength, uint64_t t_offset) const;

        //-------------------------------------------------
        // Ebo
        //-------------------------------------------------

        /**
         * @brief Add an index buffer and fill with the indices.
         *        The function sets draw count to the number of indices.
         * @param t_indices The indices.
         */
        void AddIndexBuffer(const IndexContainer& t_indices);

        //-------------------------------------------------
        // Draw
        //-------------------------------------------------

        void DrawPrimitives(uint32_t t_drawMode = GL_TRIANGLES) const;
        void DrawInstanced(int32_t t_instanceCount, uint32_t t_drawMode = GL_TRIANGLES) const;

    protected:

    private:
        /**
         * @brief To store our Vertex Array Object (Vao) Id.
         */
        uint32_t m_vaoId{ 0 };

        /**
         * @brief To store our Vertex Buffer Objects (VBo) Ids.
         */
        VboContainer m_vbos;

        /**
         * @brief To store our Ebo Id if necessary.
         */
        uint32_t m_eboId{ 0 };

        /**
         * @brief Number of elements to be rendered.
         */
        int32_t m_drawCount{ 0 };

        //-------------------------------------------------
        // Vao
        //-------------------------------------------------

        void GenerateVao();
        void DeleteVao() const;

        //-------------------------------------------------
        // Vbos
        //-------------------------------------------------

        void DeleteVbos() const;

        //-------------------------------------------------
        // Ebo
        //-------------------------------------------------

        void GenerateEbo();
        void BindEbo() const;
        void DeleteEbo() const;

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
