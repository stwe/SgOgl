#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <memory>
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
        using TerrainContainer = std::vector<std::unique_ptr<terrain::Terrain>>;

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

        void Render(TerrainContainer& t_terrains, const std::string& t_shaderProgramName) const;
        void RenderNormals(TerrainContainer& t_terrains, const std::string& t_shaderProgramName) const;

    protected:

    private:
        resource::ShaderManager& m_shaderManager;
        resource::TextureManager& m_textureManager;
        camera::LookAtCamera& m_camera;
        glm::mat4 m_projectionMatrix;
    };
}
