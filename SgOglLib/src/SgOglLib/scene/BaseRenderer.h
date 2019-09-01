#pragma once

#include <glm/mat4x4.hpp>
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderManager;
}

namespace sg::ogl::scene
{
    class Scene;

    class SG_OGL_API BaseRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BaseRenderer(
            resource::ShaderManager& t_shaderManager,
            glm::mat4& t_projection
        );

        BaseRenderer(const BaseRenderer& t_other) = delete;
        BaseRenderer(BaseRenderer&& t_other) noexcept = delete;
        BaseRenderer& operator=(const BaseRenderer& t_other) = delete;
        BaseRenderer& operator=(BaseRenderer&& t_other) noexcept = delete;

        virtual ~BaseRenderer() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Scene* GetParentScene() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParentScene(Scene* t_scene);

    protected:
        resource::ShaderManager& m_shaderManager;
        glm::mat4 m_projectionMatrix;

        Scene* m_parentScene{ nullptr };

    private:

    };
}
