// This file is part of the SgOgl package.
// 
// Filename: Terrain.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Color.h"
#include "Config.h"
#include "Application.h"
#include "Log.h"
#include "SgOglException.h"
#include "Core.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "buffer/BufferLayout.h"

namespace sg::ogl::resource
{
    class Mesh;
}

namespace sg::ogl::terrain
{
    class Terrain
    {
    public:
        using VertexContainer = std::vector<float>;
        using IndexContainer = std::vector<uint32_t>;
        using MeshUniquePtr = std::unique_ptr<resource::Mesh>;
        using HeightContainer = std::vector<std::vector<float>>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(Application* t_application, const std::string& t_configFileName);

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
        auto GetNormalmapTextureId() const { return m_normalmapTextureId; }
        auto GetSplatmapTextureId() const { return m_splatmapTextureId; }

        const resource::Mesh& GetMesh() const;

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        template <typename TNormalmapShader, typename TSplatmapShader>
        void GenerateTerrain()
        {
            // Generate a OpenGL texture handle for the heightmap.
            m_heightmapTextureId = m_application->GetTextureManager().GetTextureIdFromPath(m_terrainOptions.heightmapPath);

            // Generate OpenGL texture handles for other textures.
            for (const auto& entry : m_terrainOptions.textureContainer)
            {
                m_application->GetTextureManager().GetTextureIdFromPath(entry.second);
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
            VertexContainer vertices;
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
            IndexContainer indices;
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

            // Set the BufferLayout.
            const buffer::BufferLayout bufferLayout{
                { buffer::VertexAttributeType::POSITION, "aPosition" },
                { buffer::VertexAttributeType::NORMAL, "aNormal" },
                { buffer::VertexAttributeType::UV, "aUv" }
            };

            // Create a Mesh instance.
            m_mesh = std::make_unique<resource::Mesh>();
            SG_OGL_CORE_ASSERT(m_mesh, "[Terrain::GenerateTerrain()] Null pointer.")

            // Add Vbo.
            m_mesh->GetVao().AddVertexDataVbo(vertices.data(), count * count, bufferLayout);

            // Add an Ebo.
            m_mesh->GetVao().AddIndexBuffer(indices);

            // Render normal- and splatmap.
            ComputeMaps<TNormalmapShader, TSplatmapShader>();
        }

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        float GetHeightAtWorldPosition(float t_worldX, float t_worldZ, const glm::vec3& t_scale);
        float GetHeightAtWorldPosition(float t_worldX, float t_worldZ);

    protected:

    private:
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };

        Application* m_application{ nullptr };

        TerrainOptions m_terrainOptions{};

        uint32_t m_heightmapTextureId{ 0 };
        int32_t m_heightmapWidth{ 0 };
        int32_t m_heightmapHeight{ 0 };
        uint32_t m_normalmapTextureId{ 0 };
        uint32_t m_splatmapTextureId{ 0 };

        /**
         * @brief The ownership of the mesh is never shared.
         *        The class returns only a constant reference to
         *        the dereferenced mesh.
         */
        MeshUniquePtr m_mesh;

        HeightContainer m_terrainHeights;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        template <typename TNormalmapShader, typename TSplatmapShader>
        void ComputeMaps()
        {
            // get normalmap ShaderProgram
            m_application->GetShaderManager().AddComputeShaderProgram<TNormalmapShader>(m_terrainOptions.normalmapShaderName);
            auto& normalmapShaderProgram{ m_application->GetShaderManager().GetComputeShaderProgram(m_terrainOptions.normalmapShaderName) };

            // get splatmap ShaderProgram
            m_application->GetShaderManager().AddComputeShaderProgram<TSplatmapShader>(m_terrainOptions.splatmapShaderName);
            auto& splatmapShaderProgram{ m_application->GetShaderManager().GetComputeShaderProgram(m_terrainOptions.splatmapShaderName) };

            // render Normalmap
            m_normalmapTextureId = m_application->GetTextureManager().GetTextureId(m_terrainOptions.normalmapTextureName);
            resource::TextureManager::Bind(m_normalmapTextureId);
            resource::TextureManager::UseBilinearFilter();

            const auto numMipmaps{ log(GetHeightmapWidth()) / log(2) };
            glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, GetHeightmapWidth(), GetHeightmapWidth());

            normalmapShaderProgram.Bind();

            normalmapShaderProgram.UpdateUniforms(*this);

            glBindImageTexture(0, m_normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
            glDispatchCompute(GetHeightmapWidth() / 16, GetHeightmapWidth() / 16, 1);

            resource::ShaderProgram::Unbind();

            // render Splatmap
            m_splatmapTextureId = m_application->GetTextureManager().GetTextureId(m_terrainOptions.splatmapTextureName);
            resource::TextureManager::Bind(m_splatmapTextureId);
            resource::TextureManager::UseBilinearFilter();

            glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, GetHeightmapWidth(), GetHeightmapWidth());

            splatmapShaderProgram.Bind();

            splatmapShaderProgram.UpdateUniforms(*this);

            glBindImageTexture(0, m_splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
            glDispatchCompute(GetHeightmapWidth() / 16, GetHeightmapWidth() / 16, 1);

            resource::ShaderProgram::Unbind();
        }

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;

        static float BarryCentric(const glm::vec3& t_p1, const glm::vec3& t_p2, const glm::vec3& t_p3, const glm::vec2& t_pos);
    };
}
