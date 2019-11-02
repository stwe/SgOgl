#pragma once

#include "OpenGl.h"
#include "scene/RenderConfig.h"

namespace sg::ogl::scene::component
{
    class GuiRenderConfig : public RenderConfig
    {
    public:
        explicit GuiRenderConfig(const std::string& t_shaderFolder)
            : RenderConfig(t_shaderFolder)
        {
            drawMode = GL_TRIANGLE_STRIP;
        }

        void InitRendering() override {}

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::DisableDepthTesting();
        }

        void FinishRendering() override
        {
            OpenGl::EnableDepthTesting();
            OpenGl::DisableBlending();
        }

    protected:

    private:

    };
}
