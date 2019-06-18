#pragma once

#include <cstdint>
#include <vector>

struct Vertex
{
};

namespace sg::ogl::buffer
{
    class Vao
    {
    public:
        using Vbos = std::vector<uint32_t>;
        using IndicesContainer = std::vector<uint32_t>;
        using VerticesContainer = std::vector<Vertex>;

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

        auto GetVaoId() const;
        const auto& GetVbos() const;
        auto GetEboId() const;
        auto HasIndexBuffer() const;

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

        auto GenerateVbo();
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

        /**
         * @brief Creates and fills a vertex buffer (VBO).
         * @param t_vertices The vertices to store in the buffer.
         */
        void AllocateVertices(const VerticesContainer& t_vertices);

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
