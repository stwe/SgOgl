#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Color.h"
#include "Core.h"
#include "Config.h"

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
    class Mesh;
}

namespace sg::ogl::terrain
{
    class SG_OGL_API Terrain
    {
    public:
        using VerticesContainer = std::vector<float>;
        using IndicesContainer = std::vector<uint32_t>;
        using MeshUniquePtr = std::unique_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(
            resource::TextureManager& t_textureManager,
            resource::ShaderManager& t_shaderManager,
            const std::string& t_configFileName
        );

        Terrain(const Terrain& t_other) = delete;
        Terrain(Terrain&& t_other) noexcept = delete;
        Terrain& operator=(const Terrain& t_other) = delete;
        Terrain& operator=(Terrain&& t_other) noexcept = delete;

        ~Terrain() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const TerrainOptions& GetTerrainOptions() const;

        const MeshUniquePtr& GetMesh() const;
        MeshUniquePtr& GetMesh();

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        void GenerateTerrain();

    protected:

    private:
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };

        resource::TextureManager& m_textureManager;
        resource::ShaderManager& m_shaderManager;

        TerrainOptions m_terrainOptions{};

        MeshUniquePtr m_mesh;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;
    };
}
