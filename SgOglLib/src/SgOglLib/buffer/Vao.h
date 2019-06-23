#pragma once

#include <cstdint>
#include <vector>
#include "Core.h"

namespace sg::ogl::buffer
{
    class BufferLayout;

    class SG_OGL_API Vao
    {
    public:
        using Vbos = std::vector<uint32_t>;
        using IndicesContainer = std::vector<uint32_t>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Vao();

        Vao(const Vao& t_other) = delete;
        Vao(Vao&& t_other) noexcept = delete;
        Vao& operator=(const Vao& t_other) = delete;
        Vao& operator=(Vao&& t_other) noexcept = delete;

        ~Vao() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetVaoId() const;
        const Vbos& GetVbos() const;
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
        void DeleteVao() const;

        //-------------------------------------------------
        // Vbo`s
        //-------------------------------------------------

        uint32_t GenerateVbo();
        void BindVbo(uint32_t t_id) const;
        static void UnbindVbo();
        void DeleteVbos() const;

        //-------------------------------------------------
        // Ebo
        //-------------------------------------------------

        void GenerateEbo();
        void BindEbo() const;
        void DeleteEbo() const;

        //-------------------------------------------------
        // Allocate
        //-------------------------------------------------

        void AllocateIndices(const IndicesContainer& t_indices);
        void AllocateVertices(float* t_vertices, int32_t t_drawCount, uint32_t t_size, const BufferLayout& t_bufferLayout);

        //-------------------------------------------------
        // Draw
        //-------------------------------------------------

        void DrawPrimitives() const;

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;

    protected:

    private:
        /**
        * @brief To store our Vertex Array Object (Vao) Id.
        */
        uint32_t m_vaoId{ 0 };

        /**
         * @brief To store our Vertex Buffer Objects (VBo) Ids.
         */
        Vbos m_vbos;

        /**
         * @brief To store our Ebo Id if necessary.
         */
        uint32_t m_eboId{ 0 };

        /**
         * @brief Number of elements to be rendered.
         */
        int32_t m_drawCount{ 0 };

        void GenerateVao();
    };
}
