#pragma once

#include <glm/mat4x4.hpp>
#include <string>

namespace sg::ogl::resource
{
    class Mesh;
    class ShaderManager;
    class TextureManager;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::renderer
{
    class SkyboxRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SkyboxRenderer() = delete;

        SkyboxRenderer(
            resource::ShaderManager& t_shaderManager,
            resource::TextureManager& t_textureManager,
            camera::LookAtCamera& t_camera,
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
        resource::TextureManager& m_textureManager;
        camera::LookAtCamera& m_camera;
        glm::mat4 m_projectionMatrix;
    };
}
