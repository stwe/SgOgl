#include "Terrain.h"
#include "resource/Mesh.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "resource/stb_image.h"
#include "buffer/BufferLayout.h"
#include "Log.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Terrain::Terrain(resource::TextureManager& t_textureManager, const std::string& t_configFileName)
    : m_textureManager{ t_textureManager }
{
    Config::LoadOptions(t_configFileName, m_terrainOptions);
}

sg::ogl::terrain::Terrain::~Terrain() noexcept
{
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::TerrainOptions& sg::ogl::terrain::Terrain::GetTerrainOptions() const
{
    return m_terrainOptions;
}

sg::ogl::terrain::Terrain::MeshSharedPtr& sg::ogl::terrain::Terrain::GetMesh()
{
    return m_mesh;
}

//-------------------------------------------------
// Generate
//-------------------------------------------------

void sg::ogl::terrain::Terrain::GenerateTerrain()
{
    // Generate a OpenGL texture handle for the heightmap.
    m_heightmapTextureId = m_textureManager.GetTextureIdFromPath(m_terrainOptions.heightmapPath);

    // Generate OpenGL texture handles for other textures.
    for (const auto& entry : m_terrainOptions.texturePack)
    {
        m_textureManager.GetTextureIdFromPath(entry.second);
    }

    // Load heightmap locally again as image.
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Load heightmap {}.", m_terrainOptions.heightmapPath);
    int nrChannels;
    auto* const image{ stbi_load(m_terrainOptions.heightmapPath.c_str(), &m_heightmapWidth, &m_heightmapHeight, &nrChannels, 0) };
    if (!image)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Heightmap failed to load at path " + m_terrainOptions.heightmapPath + ".");
    }

    // Width and height should have the same value.
    if (m_heightmapWidth != m_heightmapHeight)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Width and height should have the same value.");
    }

    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] The heightmap {} was successfully loaded.", m_terrainOptions.heightmapPath);
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Heightmap width: {}, height: {}.", m_heightmapWidth, m_heightmapHeight);

    // width = height -> can use one of them as a counter
    const auto count{ m_heightmapWidth };

    // to save the generated heights of the terrain
    m_terrainHeights.resize(count, std::vector<float>(count));

    // Create the terrain vertices.
    VerticesContainer vertices;
    for (auto z{ 0 }; z < count; ++z)
    {
        for (auto x{ 0 }; x < count; ++x)
        {
            // Get the height value from the heightmap.
            auto yPos{ GetHeight(x, z, count, nrChannels, image) };

            // Scale the height value into range <0.0, 1.0>.
            if (nrChannels == STBI_grey)
            {
                yPos /= 256.0f;
            }
            else
            {
                yPos /= MAX_PIXEL_COLOUR;
            }

            SG_OGL_CORE_ASSERT(yPos >= 0.0f && yPos <= 1.0f, "[Terrain::GenerateTerrain()] Invalid height value.")

            // Save height value.
            m_terrainHeights[x][z] = yPos;

            // Scale points into range <-0.5, 0.5> on x and z axis
            auto xPos{ static_cast<float>(x) / (static_cast<float>(count) - 1) };
            auto zPos{ static_cast<float>(z) / (static_cast<float>(count) - 1) };
            xPos = -0.5f + xPos;
            zPos = -0.5f + zPos;

            SG_OGL_CORE_ASSERT(xPos >= -0.5f && xPos <= 0.5f, "[Terrain::GenerateTerrain()] Invalid x value.")
            SG_OGL_CORE_ASSERT(zPos >= -0.5f && zPos <= 0.5f, "[Terrain::GenerateTerrain()] Invalid z value.")

            // save position (3 floats)
            vertices.push_back(xPos * m_terrainOptions.scaleXz);
            vertices.push_back(yPos * m_terrainOptions.scaleY);
            vertices.push_back(zPos * m_terrainOptions.scaleXz);

            // save normal (3 floats)
            auto normal{ CalculateNormal(x, z, count, nrChannels, image) };
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            // save uv (2 floats)
            vertices.push_back(static_cast<float>(x) / (static_cast<float>(count) - 1));
            vertices.push_back(static_cast<float>(z) / (static_cast<float>(count) - 1));
        }
    }

    // Free the image memory.
    stbi_image_free(image);

    // Create the terrain indices.
    IndicesContainer indices;
    for (auto gz{ 0 }; gz < count - 1; ++gz)
    {
        for (auto gx{ 0 }; gx < count - 1; ++gx)
        {
            const auto topLeft = (gz * count) + gx;
            const auto topRight = topLeft + 1;
            const auto bottomLeft = ((gz + 1) * count) + gx;
            const auto bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Use a predefined BufferLayout.
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" }
    };

    // Create a Mesh instance.
    m_mesh.reset(new resource::Mesh);
    SG_OGL_CORE_ASSERT(m_mesh, "[Terrain::GenerateTerrain()] Null pointer.")

    // Add Vbo.
    m_mesh->GetVao().AddVertexDataVbo(vertices.data(), count * count, bufferLayout);

    // Add Ebo.
    m_mesh->GetVao().AddIndexBuffer(indices);
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
