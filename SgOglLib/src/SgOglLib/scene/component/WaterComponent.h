#pragma once

#include <cstdint>
#include "Core.h"
#include "scene/Component.h"

namespace sg::ogl::scene::component
{
    class SG_OGL_API WaterComponent : public Component
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        uint32_t reflectionTextureId{ 0 };
        uint32_t refractionTextureId{ 0 };

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
        void Update() override {}
        void Render() override {}

    protected:

    private:

    };
}
