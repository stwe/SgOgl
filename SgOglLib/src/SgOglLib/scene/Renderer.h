#pragma once

#include <glm/mat4x4.hpp>
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderManager;
}

namespace sg::ogl::scene
{
    class Node;
    class Scene;

    class SG_OGL_API Renderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Renderer() = delete;

        Renderer(
            resource::ShaderManager& t_shaderManager,
            glm::mat4& t_projection
        );

        Renderer(const Renderer& t_other) = delete;
        Renderer(Renderer&& t_other) noexcept = delete;
        Renderer& operator=(const Renderer& t_other) = delete;
        Renderer& operator=(Renderer&& t_other) noexcept = delete;

        ~Renderer() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Scene* GetParentScene() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParentScene(Scene* t_scene);

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(Node& t_node) const;

    protected:

    private:
        resource::ShaderManager& m_shaderManager;
        glm::mat4 m_projectionMatrix;

        Scene* m_parentScene{ nullptr };
    };
}
