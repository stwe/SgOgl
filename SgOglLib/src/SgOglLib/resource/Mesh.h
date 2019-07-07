#pragma once

#include <memory>
#include <vector>
#include "Log.h"
#include "buffer/Vao.h"
#include "buffer/BufferLayout.h"

namespace sg::ogl::resource
{
    struct Material;

    class SG_OGL_API Mesh
    {
    public:
        using IndicesContainer = std::vector<uint32_t>;
        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using MaterialUniquePtr = std::unique_ptr<Material>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Mesh();

        Mesh(const Mesh& t_other) = delete;
        Mesh(Mesh&& t_other) noexcept = delete;
        Mesh& operator=(const Mesh& t_other) = delete;
        Mesh& operator=(Mesh&& t_other) noexcept = delete;

        ~Mesh() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        MaterialUniquePtr& GetMaterial() noexcept;
        const MaterialUniquePtr& GetMaterial() const noexcept;

        //-------------------------------------------------
        // Allocate
        //-------------------------------------------------

        template <typename T>
        void Allocate(
            const buffer::BufferLayout& t_bufferLayout,
            std::vector<T>* t_verticesContainer,
            const int32_t t_vertexCount,
            IndicesContainer* t_indicesContainer = nullptr
        )
        {
            SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Allocate()] Null pointer.")
            SG_OGL_CORE_ASSERT(t_verticesContainer, "[Mesh::Allocate()] Null pointer.")
            SG_OGL_CORE_ASSERT(t_vertexCount > 0, "[Mesh::Allocate()] Invalid vertex count.")

            m_vao->AllocateVertices(
                reinterpret_cast<float*>(t_verticesContainer->data()),
                t_vertexCount,
                t_bufferLayout.GetFloatsPerLayout() * t_vertexCount * sizeof(float), // todo
                t_bufferLayout
            );

            if (t_indicesContainer)
            {
                m_vao->AllocateIndices(*t_indicesContainer);
            }
        }

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetMaterial(MaterialUniquePtr t_material);

        //-------------------------------------------------
        // Draw mesh
        //-------------------------------------------------

        /**
         * @brief Bind VAO.
         */
        void InitDraw() const;

        /**
         * @brief Calls glDrawElements or glDrawArrays to render.
         */
        void DrawPrimitives() const;

        /**
         * @brief Unbind VAO.
         */
        static void EndDraw();

    protected:

    private:
        VaoUniquePtr m_vao;
        MaterialUniquePtr m_material;
    };
}
