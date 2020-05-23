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
#include <glm/vec3.hpp>
#include <vector>
#include <entt/entt.hpp>

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

        std::vector<entt::id_type> toReflectionTexture;
        std::vector<entt::id_type> toRefractionTexture;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Water() = delete;

        Water(
            Application* t_application,
            float t_xPos,
            float t_zPos,
            float t_height,
            const glm::vec3& t_tileSize,
            const std::string& t_dudvMapFilePath,
            const std::string& t_normalMapFilePath
        );

        Water(const Water& t_other) = delete;
        Water(Water&& t_other) noexcept = delete;
        Water& operator=(const Water& t_other) = delete;
        Water& operator=(Water&& t_other) noexcept = delete;

        ~Water() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] float& GetXPosition();
        [[nodiscard]] float& GetZPosition();
        [[nodiscard]] float& GetHeight();
        [[nodiscard]] glm::vec3& GetTileSize();

        [[nodiscard]] float GetWaveSpeed() const;

        [[nodiscard]] float& GetWaveStrength();
        [[nodiscard]] float& GetShineDamper();
        [[nodiscard]] float& GetReflectivity();
        [[nodiscard]] glm::vec3& GetWaterColor();

        [[nodiscard]] uint32_t GetDudvTextureId() const;
        [[nodiscard]] uint32_t GetNormalTextureId() const;

        [[nodiscard]] const buffer::WaterFbos& GetWaterFbos() const;

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
        glm::vec3 m_tileSize{ 64.0f };

        float m_waveSpeed{ 0.04f };

        float m_waveStrength{ 0.01f };
        float m_shineDamper{ 30.0f };
        float m_reflectivity{ 0.9f };
        glm::vec3 m_waterColor{ glm::vec3(0.0, 0.3, 0.5) };

        uint32_t m_dudvTextureId{ 0 };
        uint32_t m_normalTextureId{ 0 };

        WaterFbosUniquePtr m_waterFbos;
    };
}
