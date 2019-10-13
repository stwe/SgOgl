#pragma once

#include "OpenGl.h"
#include "Core.h"
#include "scene/RenderConfig.h"

namespace sg::ogl::scene::component
{
    class SG_OGL_API SkyboxRenderConfig : public RenderConfig
    {
    public:
        explicit SkyboxRenderConfig(const std::string& t_shaderFolder)
            : RenderConfig(t_shaderFolder)
        {}

        void InitRendering() override {}

        void PrepareRendering() override
        {
            glDepthFunc(GL_LEQUAL);
        }

        void FinishRendering() override
        {
            // GL_LESS is the initial depth comparison function
            glDepthFunc(GL_LESS);
        }

    protected:

    private:

    };
}
