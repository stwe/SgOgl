#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::renderer
{
    class SG_OGL_API TerrainRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainRenderer() = delete;

        TerrainRenderer(
            resource::ShaderManager& t_shaderManager,
            resource::TextureManager& t_textureManager,
            camera::LookAtCamera& t_camera,
            glm::mat4& t_projection
        );

        TerrainRenderer(const TerrainRenderer& t_other) = delete;
        TerrainRenderer(TerrainRenderer&& t_other) noexcept = delete;
        TerrainRenderer& operator=(const TerrainRenderer& t_other) = delete;
        TerrainRenderer& operator=(TerrainRenderer&& t_other) noexcept = delete;

        ~TerrainRenderer() noexcept = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(const terrain::Terrain& t_terrain, const std::string& t_shaderProgramName) const;
        void RenderNormals(const terrain::Terrain& t_terrain, const std::string& t_shaderProgramName, float t_normalLength) const;

    protected:

    private:
        resource::ShaderManager& m_shaderManager;
        resource::TextureManager& m_textureManager;
        camera::LookAtCamera& m_camera;
        glm::mat4 m_projectionMatrix;
    };
}
