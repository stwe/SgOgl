#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class Mesh;
    class ShaderManager;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::renderer
{
    class SG_OGL_API SkyboxRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SkyboxRenderer() = delete;

        SkyboxRenderer(
            resource::ShaderManager& t_shaderManager,
            scene::Scene& t_scene,
            glm::mat4& t_projection
        );

        SkyboxRenderer(const SkyboxRenderer& t_other) = delete;
        SkyboxRenderer(SkyboxRenderer&& t_other) noexcept = delete;
        SkyboxRenderer& operator=(const SkyboxRenderer& t_other) = delete;
        SkyboxRenderer& operator=(SkyboxRenderer&& t_other) noexcept = delete;

        ~SkyboxRenderer() noexcept = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(uint32_t t_textureId, resource::Mesh& t_mesh, const std::string& t_shaderProgramName) const;

    protected:

    private:
        resource::ShaderManager& m_shaderManager;
        scene::Scene& m_scene;
        glm::mat4 m_projectionMatrix;
    };
}
