// This file is part of the SgOgl package.
// 
// Filename: Mesh.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>
#include <vector>
#include "Log.h"
#include "OpenGl.h"
#include "buffer/Vao.h"

namespace sg::ogl::resource
{
    struct Material;

    class Mesh
    {
    public:
        using IndexContainer = std::vector<uint32_t>;
        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using MaterialSharedPtr = std::shared_ptr<Material>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Mesh();

        explicit Mesh(const std::string& t_name);

        Mesh(const Mesh& t_other) = delete;
        Mesh(Mesh&& t_other) noexcept = delete;
        Mesh& operator=(const Mesh& t_other) = delete;
        Mesh& operator=(Mesh&& t_other) noexcept = delete;

        ~Mesh() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Getter for Mesh name.
         * @return The name of the Mesh.
         */
        std::string GetName() const;

        /**
         * @brief Get the default Material.
         * @return Material as shared_ptr.
         */
        MaterialSharedPtr GetDefaultMaterial() const;

        /**
         * @brief Get the Vao.
         * @return Reference to the Vao.
         */
        buffer::Vao& GetVao() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        /**
         * @brief Setter for Mesh name.
         * @param t_name The name of the Mesh.
         */
        void SetName(const std::string& t_name);

        /**
         * @brief Set the default Material for the Mesh.
         * @param t_defaultMaterial The default Material as shared_ptr.
         */
        void SetDefaultMaterial(const MaterialSharedPtr& t_defaultMaterial);

        //-------------------------------------------------
        // Draw - methods created for convenience
        //-------------------------------------------------

        /**
         * @brief Bind Vao.
         */
        void InitDraw() const;

        /**
         * @brief Calls glDrawElements or glDrawArrays to render.
         * @param t_drawMode Specifies what kind of primitives to render.
         */
        void DrawPrimitives(uint32_t t_drawMode = GL_TRIANGLES) const;

        /**
         * @brief Calls glDrawElementsInstanced or glDrawArraysInstanced to render.
         * @param t_instanceCount Specifies the number of instances to be rendered.
         * @param t_drawMode Specifies what kind of primitives to render.
         */
        void DrawInstanced(int32_t t_instanceCount, uint32_t t_drawMode = GL_TRIANGLES) const;

        /**
         * @brief Unbind Vao.
         */
        static void EndDraw();

    protected:

    private:
        /**
         * @brief The Vao of the Mesh.
         */
        VaoUniquePtr m_vao;

        /**
         * @brief The name of the Mesh.
         *        The name is used as a key if the mesh is to be
         *        stored in the ModelManager.
         */
        std::string m_name;

        /**
         * @brief The default Material of the Mesh.
         */
        MaterialSharedPtr m_defaultMaterial;
    };
}
