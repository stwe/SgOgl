#pragma once

#include <string>
#include "BaseRenderer.h"

namespace sg::ogl::resource
{
    class Mesh;
}

namespace sg::ogl::scene
{
    class SG_OGL_API SkyboxRenderer : public BaseRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SkyboxRenderer() = delete;

        SkyboxRenderer(
            resource::ShaderManager& t_shaderManager,
            glm::mat4& t_projection
        );

        SkyboxRenderer(const SkyboxRenderer& t_other) = delete;
        SkyboxRenderer(SkyboxRenderer&& t_other) noexcept = delete;
        SkyboxRenderer& operator=(const SkyboxRenderer& t_other) = delete;
        SkyboxRenderer& operator=(SkyboxRenderer&& t_other) noexcept = delete;

        ~SkyboxRenderer() noexcept override = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(uint32_t t_textureId, resource::Mesh& t_mesh, const std::string& t_shaderProgramName) const;

    protected:

    private:

    };
}
