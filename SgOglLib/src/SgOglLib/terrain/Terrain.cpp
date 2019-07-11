#include <vector>
#include "Terrain.h"
#include "resource/Mesh.h"
#include "resource/TextureManager.h"
#include "resource/stb_image.h"
#include "buffer/BufferLayout.h"
#include "Log.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Terrain::Terrain(const float t_posX, const  float t_posZ, resource::TextureManager& t_textureManager, const std::string& t_heightmapPath)
    : m_textureManager{ t_textureManager }
    , m_posX{ t_posX * SIZE }
    , m_posZ{ t_posZ * SIZE }
{
    GenerateTerrain(t_heightmapPath);
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

float sg::ogl::terrain::Terrain::GetPosX() const
{
    return m_posX;
}

float sg::ogl::terrain::Terrain::GetPosZ() const
{
    return m_posZ;
}

const sg::ogl::terrain::Terrain::MeshUniquePtr& sg::ogl::terrain::Terrain::GetMesh() const
{
    return m_mesh;
}

sg::ogl::terrain::Terrain::MeshUniquePtr& sg::ogl::terrain::Terrain::GetMesh()
{
    return m_mesh;
}

uint32_t sg::ogl::terrain::Terrain::GetTextureId() const
{
    return m_grassTextureId;
}

//-------------------------------------------------
// Generate
//-------------------------------------------------

void sg::ogl::terrain::Terrain::GenerateTerrain(const std::string& t_heightmapPath)
{
    // load heightmap
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Load heightmap {}.", t_heightmapPath);
    int nrChannels, width, height;
    const auto* const image{ stbi_load(t_heightmapPath.c_str(), &width, &height, &nrChannels, 0) };
    if (!image)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Heightmap failed to load at path " + t_heightmapPath + ".");
    }

    // width and height should have the same value
    if (width != height)
    {
        throw SG_OGL_EXCEPTION("[Terrain::GenerateTerrain()] Width and height should have the same value.");
    }

    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] The heightmap {} was successfully loaded.", t_heightmapPath);
    SG_OGL_CORE_LOG_DEBUG("[Terrain::GenerateTerrain()] Heightmap width: {}, height: {}.", width, height);

    // width = height -> can use one of them as a counter
    const auto count{ height };

    // create vertices
    VerticesContainer vertices;
    for (auto z{ 0 }; z < count; ++z)
    {
        for (auto x{ 0 }; x < count; ++x)
        {
            // get the height value from the heightmap
            auto y{ GetHeight(x, z, count, nrChannels, image) };

            // scale the height value into range <0.0, 1.0>
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
            vertices.push_back(static_cast<float>(x) / (static_cast<float>(count) - 1) * SIZE);
            vertices.push_back(y * SCALE_HEIGHT);
            vertices.push_back(static_cast<float>(z) / (static_cast<float>(count) - 1) * SIZE);

            // normal (3 floats)
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // uv (2 floats)
            vertices.push_back(static_cast<float>(x) / (static_cast<float>(count) - 1));
            vertices.push_back(static_cast<float>(z) / (static_cast<float>(count) - 1));
        }
    }

    // create indices
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

    // Load grass texture.
    m_grassTextureId = m_textureManager.GetTextureIdFromPath("res/texture/grass.jpg");
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
