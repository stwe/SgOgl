#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include "Color.h"
#include "Core.h"
#include "resource/ComputeShaderTexture.h"

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

        using TextureName = std::string;
        using TexturePath = std::string;
        using TexturePack = std::map<TextureName, TexturePath>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(
            float t_posX,
            float t_posZ,
            resource::TextureManager& t_textureManager,
            resource::ShaderManager& t_shaderManager,
            resource::ComputeShaderTexture& t_normalmap,
            resource::ComputeShaderTexture& t_splatmap,
            const std::string& t_heightmapPath,
            TexturePack t_texturePack
        );

        Terrain(const Terrain& t_other) = delete;
        Terrain(Terrain&& t_other) noexcept = delete;
        Terrain& operator=(const Terrain& t_other) = delete;
        Terrain& operator=(Terrain&& t_other) noexcept = delete;

        ~Terrain() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        float GetPosX() const;
        float GetPosZ() const;

        const MeshUniquePtr& GetMesh() const;
        MeshUniquePtr& GetMesh();

        const TexturePack& GetTexturePack() const;

        uint32_t GetHeightmapTextureId() const;

    protected:

    private:
        static constexpr float SIZE{ 800 };
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };
        static constexpr auto SCALE_HEIGHT{ 512.0f };

        resource::TextureManager& m_textureManager;
        resource::ShaderManager& m_shaderManager;

        resource::ComputeShaderTexture m_normalmap;
        resource::ComputeShaderTexture m_splatmap;

        float m_posX;
        float m_posZ;

        MeshUniquePtr m_mesh;

        TexturePack m_texturePack;

        int m_heightmapWidth{ 0 };
        uint32_t m_heightmapTextureId{ 0 };

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        void GenerateTerrain(const std::string& t_heightmapPath);
        void GenerateMaps(); // todo

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;
    };
}
