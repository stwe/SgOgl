#pragma once

#include "BaseRenderer.h"

namespace sg::ogl::scene
{
    class Node;

    class SG_OGL_API Renderer : public BaseRenderer
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

        ~Renderer() noexcept override = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(Node& t_node) const;
        void Render(Node& t_node, int32_t t_instanceCount) const;

    protected:

    private:

    };
}
