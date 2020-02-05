// This file is part of the SgOgl package.
// 
// Filename: TerrainConfig.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <vector>
#include <array>
#include <string>

namespace sg::ogl
{
    class Application;
}

constexpr std::array POW2{
    1, 2, 4, 8, 16,
    32, 64, 128, 256, 512
};

constexpr std::array POW2_F{
    1.0f, 2.0f, 4.0f, 8.0f, 16.0f,
    32.0f, 64.0f, 128.0f, 256.0f, 512.0f
};

namespace sg::ogl::terrain
{
    class TerrainConfig
    {
    public:
        using LodRangeContainer = std::vector<int>;
        using LodMorphingAreaContainer = std::vector<int>;
        using HeightmapHeightContainer = std::vector<float>;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        float scaleXz{ 1.0f };
        float scaleY{ 1.0f };
        int rootNodes{ 2 };

        float normalStrength{ 4.0f };

        int tessellationFactor{ 700 };
        float tessellationSlope{ 1.63f };
        float tessellationShift{ -0.5f };

        bool morphingEnabled{ true };
        bool tessellationEnabled{ true };

        LodRangeContainer lodRanges;

        bool use16BitHeightmap{ false };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainConfig() = delete;

        explicit TerrainConfig(Application* t_application);

        TerrainConfig(const TerrainConfig& t_other) = delete;
        TerrainConfig(TerrainConfig&& t_other) noexcept = delete;
        TerrainConfig& operator=(const TerrainConfig& t_other) = delete;
        TerrainConfig& operator=(TerrainConfig&& t_other) noexcept = delete;

        ~TerrainConfig() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] uint32_t GetHeightmapTextureId() const;
        [[nodiscard]] uint32_t GetNormalmapTextureId() const;
        [[nodiscard]] uint32_t GetSplatmapTextureId() const;

        [[nodiscard]] uint32_t GetSandTextureId() const;
        [[nodiscard]] uint32_t GetGrassTextureId() const;
        [[nodiscard]] uint32_t GetRockTextureId() const;
        [[nodiscard]] uint32_t GetSnowTextureId() const;

        [[nodiscard]] int GetHeightmapWidth() const;
        [[nodiscard]] const LodMorphingAreaContainer& GetLodMorphingArea() const;

        [[nodiscard]] HeightmapHeightContainer& GetHeightmapData();
        [[nodiscard]] const HeightmapHeightContainer& GetHeightmapData() const;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitMapsAndMorphing(
            const std::string& t_heightmapFilePath,
            const std::string& t_normalmapTextureName,
            const std::string& t_splatmapTextureName
        );

        void InitTextures(
            const std::string& t_sandFilePath,
            const std::string& t_grassFilePath,
            const std::string& t_rockFilePath,
            const std::string& t_snowFilePath
        );

    protected:

    private:
        Application* m_application{ nullptr };

        uint32_t m_heightmapTextureId{ 0 };
        uint32_t m_normalmapTextureId{ 0 };
        uint32_t m_splatmapTextureId{ 0 };

        uint32_t m_sandTextureId{ 0 };
        uint32_t m_grassTextureId{ 0 };
        uint32_t m_rockTextureId{ 0 };
        uint32_t m_snowTextureId{ 0 };

        int m_heightmapWidth{ 0 };

        LodMorphingAreaContainer m_lodMorphingArea;
        HeightmapHeightContainer m_heightmapData;

        //-------------------------------------------------
        // Load maps
        //-------------------------------------------------

        void LoadHeightmap(const std::string& t_heightmapFilePath);

        void LoadNormalmap(const std::string& t_normalmapTextureName);
        void LoadSplatmap(const std::string& t_splatmapTextureName);

        void InitMorphing();

        void InitHeightmapData();
    };
}
