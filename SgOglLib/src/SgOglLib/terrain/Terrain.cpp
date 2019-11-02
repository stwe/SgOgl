// This file is part of the SgOgl package.
// 
// Filename: Terrain.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Terrain.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Terrain::Terrain(Application* t_application, const std::string& t_configFileName)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[Terrain::Terrain()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[Terrain::Terrain()] Create Terrain.");

    Config::LoadOptions(t_configFileName, m_terrainOptions);
}

sg::ogl::terrain::Terrain::~Terrain() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Terrain::~Terrain()] Destruct Terrain.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::TerrainOptions& sg::ogl::terrain::Terrain::GetTerrainOptions() const
{
    return m_terrainOptions;
}

const sg::ogl::resource::Mesh& sg::ogl::terrain::Terrain::GetMesh() const
{
    if (!m_mesh)
    {
        SG_OGL_CORE_LOG_ERROR("[Terrain::GetMesh()] You may have forgotten to call the GenerateTerrain() method.");
    }

    return *m_mesh;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

float sg::ogl::terrain::Terrain::GetHeightAtWorldPosition(const float t_worldX, const float t_worldZ, const glm::vec3& t_scale)
{
    const auto newWorldX{ t_worldX + t_scale.x * 0.5f };
    const auto newWorldZ{ t_worldZ + t_scale.z * 0.5f };

    const auto scaleFactorX{ t_scale.x / m_heightmapWidth };
    const auto scaleFactorZ{ t_scale.z / m_heightmapWidth };

    const auto mapX{ static_cast<int>(newWorldX / scaleFactorX) };
    const auto mapY{ static_cast<int>(newWorldZ / scaleFactorZ) };

    if (
        mapX >= m_heightmapWidth - 1 ||
        mapY >= m_heightmapWidth - 1 ||
        mapX < 0.0f ||
        mapY < 0.0f
    )
    {
        return 0.0f;
    }

    float result;
    //result = m_terrainHeights[mapX][mapY];

    const auto xCoord{ fmod(newWorldX, scaleFactorX) / scaleFactorX };
    const auto zCoord{ fmod(newWorldZ, scaleFactorZ) / scaleFactorZ };

    if (xCoord <= (1.0f - zCoord))
    {
        result = BarryCentric(
            glm::vec3(0.0f, m_terrainHeights[mapX][mapY], 0.0f),
            glm::vec3(1.0f, m_terrainHeights[mapX + 1][mapY], 0.0f),
            glm::vec3(0.0f, m_terrainHeights[mapX][mapY + 1], 1.0f),
            glm::vec2(xCoord, zCoord)
        );
    }
    else
    {
        result = BarryCentric(
            glm::vec3(1.0f, m_terrainHeights[mapX + 1][mapY], 0.0f),
            glm::vec3(1.0f, m_terrainHeights[mapX + 1][mapY + 1], 1.0f),
            glm::vec3(0.0f, m_terrainHeights[mapX][mapY + 1], 1.0f),
            glm::vec2(xCoord, zCoord)
        );
    }

    return result * t_scale.y;
}

float sg::ogl::terrain::Terrain::GetHeightAtWorldPosition(const float t_worldX, const float t_worldZ)
{
    const auto scale{ glm::vec3(m_terrainOptions.scaleXz, m_terrainOptions.scaleY, m_terrainOptions.scaleXz) };
    return GetHeightAtWorldPosition(t_worldX, t_worldZ, scale);
}

sg::ogl::Color sg::ogl::terrain::Terrain::GetPixel(const int t_x, const int t_z, const int t_length, const int t_channels, const unsigned char* const t_image)
{
    const auto offset{ (t_x + t_z * t_length) * t_channels };

    Color color;

    if (t_channels == STBI_grey)
    {
        color.r = t_image[offset];
        color.g = t_image[offset];
        color.b = t_image[offset];
        color.a = 255;
    }
    else
    {
        color.r = t_image[offset];
        color.g = t_image[offset + 1];
        color.b = t_image[offset + 2];

        t_channels == STBI_rgb_alpha ? color.a = t_image[offset + 3] : color.a = 255;
    }

    return color;
}

float sg::ogl::terrain::Terrain::GetHeight(const int t_x, const int t_z, const int t_length, const int t_channels, const unsigned char* const t_image)
{
    // out of bound
    if (t_x < 0 || t_x >= t_length || t_z < 0 || t_z >= t_length)
    {
        return 0.0f;
    }

    const auto heightMapPixel{ GetPixel(t_x, t_z, t_length, t_channels, t_image) };

    if (t_channels == STBI_grey)
    {
        return static_cast<float>(heightMapPixel.r);
    }

    return static_cast<float>(heightMapPixel.r * heightMapPixel.g * heightMapPixel.b);
}

glm::vec3 sg::ogl::terrain::Terrain::CalculateNormal(int t_x, int t_z, const int t_length, const int t_channels, const unsigned char* t_image) const
{
    if (t_x == 0)
    {
        t_x = 1;
    }

    if (t_z == 0)
    {
        t_z = 1;
    }

    const auto heightL{ GetHeight(t_x - 1, t_z, t_length, t_channels, t_image) };
    const auto heightR{ GetHeight(t_x + 1, t_z, t_length, t_channels, t_image) };
    const auto heightD{ GetHeight(t_x, t_z - 1, t_length, t_channels, t_image) };
    const auto heightU{ GetHeight(t_x, t_z + 1, t_length, t_channels, t_image) };

    const auto normal{ glm::vec3(heightL - heightR, 2.0f, heightD - heightU) };

    return normalize(normal);
}

float sg::ogl::terrain::Terrain::BarryCentric(const glm::vec3& t_p1, const glm::vec3& t_p2, const glm::vec3& t_p3, const glm::vec2& t_pos)
{
    const auto det{ (t_p2.z - t_p3.z) * (t_p1.x - t_p3.x) + (t_p3.x - t_p2.x) * (t_p1.z - t_p3.z) };
    const auto l1{ ((t_p2.z - t_p3.z) * (t_pos.x - t_p3.x) + (t_p3.x - t_p2.x) * (t_pos.y - t_p3.z)) / det };
    const auto l2{ ((t_p3.z - t_p1.z) * (t_pos.x - t_p3.x) + (t_p1.x - t_p3.x) * (t_pos.y - t_p3.z)) / det };
    const auto l3{ 1.0f - l1 - l2 };

    return l1 * t_p1.y + l2 * t_p2.y + l3 * t_p3.y;
}
