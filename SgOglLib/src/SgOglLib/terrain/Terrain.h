#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Color.h"
#include "Core.h"
#include "Config.h"

namespace sg::ogl::resource
{
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
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;
        using HeightContainer = std::vector<std::vector<float>>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(resource::TextureManager& t_textureManager, const std::string& t_configFileName);

        Terrain(const Terrain& t_other) = delete;
        Terrain(Terrain&& t_other) noexcept = delete;
        Terrain& operator=(const Terrain& t_other) = delete;
        Terrain& operator=(Terrain&& t_other) noexcept = delete;

        ~Terrain() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const TerrainOptions& GetTerrainOptions() const;

        auto GetHeightmapTextureId() const { return m_heightmapTextureId; }
        auto GetHeightmapWidth() const { return m_heightmapWidth; }
        auto GetHeightmapHeight() const { return m_heightmapHeight; }

        MeshSharedPtr& GetMesh();

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        void GenerateTerrain();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        float GetHeightAtWorldPosition(float t_worldX, float t_worldZ, const glm::vec3& t_scale);
        float GetHeightAtWorldPosition(float t_worldX, float t_worldZ);

    protected:

    private:
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };

        resource::TextureManager& m_textureManager;

        TerrainOptions m_terrainOptions{};

        uint32_t m_heightmapTextureId{ 0 };
        int32_t m_heightmapWidth{ 0 };
        int32_t m_heightmapHeight{ 0 };

        MeshSharedPtr m_mesh;

        HeightContainer m_terrainHeights;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;

        static float BarryCentric(const glm::vec3& t_p1, const glm::vec3& t_p2, const glm::vec3& t_p3, const glm::vec2& t_pos);
    };
}
