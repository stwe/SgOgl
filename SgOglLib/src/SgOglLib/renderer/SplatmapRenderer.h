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
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SplatmapRenderer() = delete;

        SplatmapRenderer(int t_heightmapWidth, std::string t_splatmapTextureName, resource::TextureManager& t_textureManager);

        SplatmapRenderer(const SplatmapRenderer& t_other) = delete;
        SplatmapRenderer(SplatmapRenderer&& t_other) noexcept = delete;
        SplatmapRenderer& operator=(const SplatmapRenderer& t_other) = delete;
        SplatmapRenderer& operator=(SplatmapRenderer&& t_other) noexcept = delete;

        ~SplatmapRenderer() noexcept = default;

        //-------------------------------------------------
        // Compute splatmap
        //-------------------------------------------------

        void ComputeSplatmap(resource::ShaderManager& t_shaderManager, uint32_t t_heightmapId, float t_normalStrength) const;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetSplatmapTextureId() const;

    protected:

    private:
        /**
         * @brief The width of the normalmap.
         */
        int m_heightmapWidth{ 0 };

        /**
         * @brief The unique name of the splatmap in the TextureManager.
         */
        std::string m_splatmapTextureName;

        /**
         * @brief The id of the splatmap texture.
         */
        uint32_t m_splatmapTextureId{ 0 };
    };
}
