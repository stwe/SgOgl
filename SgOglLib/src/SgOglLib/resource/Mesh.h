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
        using IndexContainer = std::vector<uint32_t>;
        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using MaterialUniquePtr = std::unique_ptr<Material>;
        using MaterialSharedPtr = std::shared_ptr<Material>;

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

        /**
         * @brief Get the default Material as shared_ptr. Mesh has
         *        ownership of Material. The Material is also used
         *        in a Node, so later Node has also ownership.
         * @return 
         */
        MaterialSharedPtr GetDefaultMaterial() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        /**
         * @brief Set the default Material for the Mesh. The Material should come from a
         *        factory function as unique_ptr. The unique_ptr is converted to a
         *        shared_ptr (m_defaultMaterial).
         * @param t_defaultMaterial The default Material as unique_ptr.
         */
        void SetDefaultMaterial(MaterialUniquePtr t_defaultMaterial);

        //-------------------------------------------------
        // Allocate
        //-------------------------------------------------

        template <typename T>
        void Allocate(
            const buffer::BufferLayout& t_bufferLayout,
            std::vector<T>* t_vertexContainer,
            const int32_t t_vertexCount,
            IndexContainer* t_indexContainer = nullptr
        )
        {
            SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Allocate()] Null pointer.")
            SG_OGL_CORE_ASSERT(t_vertexContainer, "[Mesh::Allocate()] Null pointer.")
            SG_OGL_CORE_ASSERT(t_vertexCount > 0, "[Mesh::Allocate()] Invalid vertex count.")

            m_vao->AllocateVertices(
                reinterpret_cast<float*>(t_vertexContainer->data()),
                t_vertexCount,
                t_bufferLayout.GetFloatsPerLayout() * t_vertexCount * sizeof(float),
                t_bufferLayout
            );

            if (t_indexContainer)
            {
                m_vao->AllocateIndices(*t_indexContainer);
            }
        }

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
        /**
         * @brief The Vao of the Mesh.
         */
        VaoUniquePtr m_vao;

        /**
         * @brief The default Material of the Mesh.
         */
        MaterialSharedPtr m_defaultMaterial;
    };
}
