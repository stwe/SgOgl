#pragma once

#include "scene/RenderConfig.h"

namespace sg::ogl::scene::component
{
    class WaterRenderConfig : public RenderConfig
    {
    public:
        explicit WaterRenderConfig(const std::string& t_shaderFolder)
            : RenderConfig(t_shaderFolder)
        {}

        void InitRendering() override {}

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
        }

    protected:

    private:

    };
}
