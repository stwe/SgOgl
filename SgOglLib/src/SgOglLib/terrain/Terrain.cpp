#include "Terrain.h"
#include "resource/Mesh.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "resource/stb_image.h"
#include "buffer/BufferLayout.h"
#include "compute/NormalmapRenderer.h"
#include "compute/SplatmapRenderer.h"
#include "Log.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Terrain::Terrain(
    resource::TextureManager& t_textureManager,
    resource::ShaderManager& t_shaderManager,
    const std::string& t_configFileName
)
    : m_textureManager{ t_textureManager }
    , m_shaderManager{ t_shaderManager }
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
    const auto heightmapTextureId{ m_textureManager.GetTextureIdFromPath(m_terrainOptions.heightmapPath) };

    // Generate OpenGL texture handles for other textures.
    for (const auto& entry : m_terrainOptions.texturePack)
    {
        m_textureManager.GetTextureIdFromPath(entry.second);
    }

    // Load heightmap locally again as image.
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Load heightmap {}.", m_terrainOptions.heightmapPath);
    int nrChannels, width, height;
    auto* const image{ stbi_load(m_terrainOptions.heightmapPath.c_str(), &width, &height, &nrChannels, 0) };
    if (!image)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Heightmap failed to load at path " + m_terrainOptions.heightmapPath + ".");
    }

    // Width and height should have the same value.
    if (width != height)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Width and height should have the same value.");
    }

    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] The heightmap {} was successfully loaded.", m_terrainOptions.heightmapPath);
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Heightmap width: {}, height: {}.", width, height);

    // width = height -> can use one of them as a counter
    const auto count{ width };

    // Create the terrain vertices.
    VerticesContainer vertices;
    for (auto z{ 0 }; z < count; ++z)
    {
        for (auto x{ 0 }; x < count; ++x)
        {
            // Get the height value from the heightmap.
            auto y{ GetHeight(x, z, count, nrChannels, image) };

            // Scale the height value into range <0.0, 1.0>.
            if (nrChannels == STBI_grey)
            {
                y /= 256.0f;
            }
            else
            {
                y /= MAX_PIXEL_COLOUR;
            }

            SG_OGL_CORE_ASSERT(y >= 0.0f && y <= 1.0f, "[Terrain::GenerateTerrain()] Invalid height.")

            // position (3 floats)
            vertices.push_back(static_cast<float>(x) / (static_cast<float>(count) - 1) * m_terrainOptions.scaleXz);
            vertices.push_back(y * m_terrainOptions.scaleY);
            vertices.push_back(static_cast<float>(z) / (static_cast<float>(count) - 1) * m_terrainOptions.scaleXz);

            // normal (3 floats)
            auto normal{ CalculateNormal(x, z, count, nrChannels, image) };
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            // uv (2 floats)
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

    // Allocate the data to the mesh.
    m_mesh->Allocate(bufferLayout, &vertices, count * count, &indices);

    // Generate normalmap.
    m_terrainOptions.normalmap.m_textureId = compute::NormalmapRenderer::ComputeNormalmap(
        m_textureManager,
        m_shaderManager,
        m_terrainOptions.normalmap.uniqueTextureName,
        m_terrainOptions.normalmap.computeShaderName,
        heightmapTextureId,
        width,
        m_terrainOptions.normalStrength
    );

    // Generate splatmap.
    m_terrainOptions.splatmap.m_textureId = compute::SplatmapRenderer::ComputeSplatmap(
        m_textureManager,
        m_shaderManager,
        m_terrainOptions.splatmap.uniqueTextureName,
        m_terrainOptions.splatmap.computeShaderName,
        m_terrainOptions.normalmap.GetTextureId(),
        width
    );
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

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
