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
        float scaleXz{ 1.0f };
        float scaleY{ 1.0f };
        int rootNodes{ 2 };

        float normalStrength{ 4.0f };

        int tessellationFactor{ 600 };
        float tessellationSlope{ 1.8f };
        float tessellationShift{ 0.1f };

        bool morphingEnabled{ true };
        bool tessellationEnabled{ true };

        std::vector<int> lodRanges;

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
        [[nodiscard]] int GetHeightmapWidth() const;
        [[nodiscard]] const std::vector<int>& GetLodMorphingArea() const;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitMaps(
            const std::string& t_heightmapFilePath,
            const std::string& t_normalmapShaderName,
            const std::string& t_normalmapTextureName
        );

        void InitMorphing();

    protected:

    private:
        Application* m_application{ nullptr };

        uint32_t m_heightmapTextureId{ 0 };
        uint32_t m_normalmapTextureId{ 0 };

        int m_heightmapWidth{ 0 };

        std::vector<int> m_lodMorphingArea;
    };
}
