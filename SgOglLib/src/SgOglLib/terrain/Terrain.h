#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Color.h"
#include "Core.h"

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
        using MeshUniquePtr = std::unique_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Terrain() = delete;

        Terrain(float t_posX, float t_posZ, resource::TextureManager& t_textureManager, const std::string& t_heightmapPath);

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

        uint32_t GetTextureId() const;

    protected:

    private:
        static constexpr float SIZE{ 800 };
        static constexpr auto MAX_PIXEL_COLOUR{ 256.0f * 256.0f * 256.0f };
        static constexpr auto SCALE_HEIGHT{ 128.0f };

        resource::TextureManager& m_textureManager;

        float m_posX;
        float m_posZ;

        MeshUniquePtr m_mesh;

        uint32_t m_grassTextureId{ 0 };

        //-------------------------------------------------
        // Generate
        //-------------------------------------------------

        void GenerateTerrain(const std::string& t_heightmapPath);

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static Color GetPixel(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);
        static float GetHeight(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image);

        glm::vec3 CalculateNormal(int t_x, int t_z, int t_length, int t_channels, const unsigned char* t_image) const;
    };
}
