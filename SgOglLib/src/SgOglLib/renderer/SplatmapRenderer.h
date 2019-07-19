#pragma once

#include <string>

namespace sg::ogl::resource
{
    class TextureManager;
    class ShaderManager;
}

namespace sg::ogl::renderer
{
    class SplatmapRenderer
    {
    public:
        static uint32_t ComputeSplatmap(
            resource::TextureManager& t_textureManager,
            resource::ShaderManager& t_shaderManager,
            const std::string& t_splatmapTextureName,
            const std::string& t_splatmapShaderName,
            uint32_t t_normalmapTextureId,
            int t_heightmapWidth
        );

    protected:

    private:

    };
}
