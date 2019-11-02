#pragma once

#include <cstdint>
#include "scene/Component.h"

namespace sg::ogl::scene::component
{
    class WaterComponent : public Component
    {
    public:
        static constexpr float WAVE_SPEED{ 0.04f };

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        uint32_t reflectionTextureId{ 0 };
        uint32_t refractionTextureId{ 0 };
        uint32_t dudvTextureId{ 0 };
        uint32_t normalTextureId{ 0 };
        uint32_t depthTextureId{ 0 };

        float moveFactor{ 0.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterComponent() = default;

        WaterComponent(const WaterComponent& t_other) = delete;
        WaterComponent(WaterComponent&& t_other) noexcept = delete;
        WaterComponent& operator=(const WaterComponent& t_other) = delete;
        WaterComponent& operator=(WaterComponent&& t_other) noexcept = delete;

        ~WaterComponent() noexcept = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init() override {}
        void Input() override {}

        void Update(const double t_dt) override
        {
            moveFactor += WAVE_SPEED * static_cast<float>(t_dt);
            moveFactor = fmod(moveFactor, 1.0f);
        }

        void Render() override {}

    protected:

    private:

    };
}
