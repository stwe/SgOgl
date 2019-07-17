#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include "Color.h"
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
    class Mesh;
}

namespace sg::ogl::renderer
{
    class SplatmapRenderer;
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
        using TextureMap = std::map<TextureName, TexturePath>;

        using SplatmapRendererUniquePtr = std::unique_ptr<renderer::SplatmapRenderer>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(
            float t_posX,
            float t_posZ,
            resource::TextureManager& t_textureManager,
            resource::ShaderManager& t_shaderManager,
            const std::string& t_heightmapPath,
            TextureMap t_textureMap
        );

        Terrain(const Terrain& t_other) = delete;
        Terrain(Terrain&& t_other) noexcept = delete;
        Terrain& operator=(const Terrain& t_other) = delete;
        Terrain& operator=(Terrain&& t_other) noexcept = delete;

        ~Terrain() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        float GetPosX() const;
        float GetPosZ() const;

        const MeshUniquePtr& GetMesh() const;
        MeshUniquePtr& GetMesh();

        const TextureMap& GetTextureMap() const;

        uint32_t GetHeightmapTextureId() const;

    protected:

    private:
        static constexpr float SIZE{ 800 };
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };
        static constexpr auto SCALE_HEIGHT{ 128.0f };

        resource::TextureManager& m_textureManager;
        resource::ShaderManager& m_shaderManager;

        float m_posX;
        float m_posZ;

        MeshUniquePtr m_mesh;

        TextureMap m_textureMap;

        int m_heightmapWidth{ 0 };
        uint32_t m_heightmapTextureId{ 0 };

        SplatmapRendererUniquePtr m_splatmapRenderer;

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        void GenerateTerrain(const std::string& t_heightmapPath);
        void GenerateSplatmap();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;
    };
}
