#pragma once

#include "scene/RenderConfig.h"
#include "Core.h"

namespace sg::ogl::scene::component
{
    class SG_OGL_API ModelRenderConfig : public RenderConfig
    {
    public:
        explicit ModelRenderConfig(const std::string& t_shaderFolder)
            : RenderConfig(t_shaderFolder)
        {}

        void InitRendering() override {}
        void PrepareRendering() override {}
        void FinishRendering() override {}

    protected:

    private:

    };
}
