#pragma once

#include "BaseRenderer.h"

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::resource
{
    class TextureManager;
}

namespace sg::ogl::scene
{
    class SG_OGL_API TerrainRenderer : public BaseRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainRenderer() = delete;

        TerrainRenderer(
            resource::ShaderManager& t_shaderManager,
            resource::TextureManager& t_textureManager,
            glm::mat4& t_projection
        );

        TerrainRenderer(const TerrainRenderer& t_other) = delete;
        TerrainRenderer(TerrainRenderer&& t_other) noexcept = delete;
        TerrainRenderer& operator=(const TerrainRenderer& t_other) = delete;
        TerrainRenderer& operator=(TerrainRenderer&& t_other) noexcept = delete;

        ~TerrainRenderer() noexcept override = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(const terrain::Terrain& t_terrain) const;
        void RenderNormals(const terrain::Terrain& t_terrain, float t_normalLength) const;

    protected:

    private:
        resource::TextureManager& m_textureManager;
    };
}
