#pragma once

#include <string>

namespace sg::ogl::resource
{
    class TextureManager;
    class ShaderManager;
}

namespace sg::ogl::renderer
{
    class NormalmapRenderer
    {
    public:
        static uint32_t ComputeNormalmap(
            resource::TextureManager& t_textureManager,
            resource::ShaderManager& t_shaderManager,
            const std::string& t_normalmapTextureName,
            const std::string& t_normalmapShaderName,
            uint32_t t_heightmapTextureId,
            int t_heightmapWidth,
            float t_normalStrength
        );

    protected:

    private:

    };
}
