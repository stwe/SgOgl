#pragma once

#include <string>

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::resource
{
    class SG_OGL_LOCAL ComputeShaderTexture
    {
    public:
        friend terrain::Terrain;

        /**
         * @brief The unique name of the texture in the TextureManager.
         */
        std::string uniqueTextureName;

        /**
         * @brief The name of the compute shader.
         */
        std::string computeShaderName;

        auto GetTextureId() const { return m_textureId; }

    protected:

    private:
        /**
         * @brief Stores the id of the created texture.
         *        Can also be accessed via the TextureManager.
         */
        uint32_t m_textureId{ 0 };
    };
}
