// This file is part of the SgOgl package.
// 
// Filename: Water.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>
#include <string>
#include <memory>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::buffer
{
    class WaterFbos;
}

namespace sg::ogl::water
{
    class Water
    {
    public:
        using WaterFbosUniquePtr = std::unique_ptr<buffer::WaterFbos>;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        float moveFactor{ 0.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Water() = delete;

        Water(
            Application* t_application,
            float t_xPos,
            float t_zPos,
            float t_height,
            float t_tileSize,
            const std::string& t_dudvMapFilePath,
            const std::string& t_normalMapFilePath
        );

        Water(const Water& t_other) = delete;
        Water(Water&& t_other) noexcept = delete;
        Water& operator=(const Water& t_other) = delete;
        Water& operator=(Water&& t_other) noexcept = delete;

        ~Water();

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        float GetXPosition() const;
        float GetZPosition() const;
        float GetHeight() const;
        float GetTileSize() const;

        float GetWaveSpeed() const;

        uint32_t GetDudvTextureId() const;
        uint32_t GetNormalTextureId() const;

        const buffer::WaterFbos& GetWaterFbos() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetWaveSpeed(float t_waveSpeed);

    protected:

    private:
        Application* m_application{ nullptr };

        float m_xPos{ 0.0f };
        float m_zPos{ 0.0f };
        float m_height{ 0.0f };
        float m_tileSize{ 64.0f };

        float m_waveSpeed{ 0.04f };

        uint32_t m_dudvTextureId{ 0 };
        uint32_t m_normalTextureId{ 0 };

        WaterFbosUniquePtr m_waterFbos;
    };
}
