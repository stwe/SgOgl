// This file is part of the SgOgl package.
// 
// Filename: Sun.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::light
{
    struct Sun : DirectionalLight
    {
        static constexpr auto SUN_DISTANCE{ 60.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Sun() = default;

        Sun(
            const glm::vec3& t_direction,
            const glm::vec3& t_diffuseIntensity,
            const glm::vec3& t_specularIntensity,
            const uint32_t t_textureId, const float t_scale
        )
            : DirectionalLight(t_direction, t_diffuseIntensity, t_specularIntensity)
            , textureId{ t_textureId }
            , scale{ t_scale }
        {}

        Sun(const Sun& t_other) = default;
        Sun(Sun&& t_other) noexcept = default;
        Sun& operator=(const Sun& t_other) = default;
        Sun& operator=(Sun&& t_other) noexcept = default;

        virtual ~Sun() noexcept = default;

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        uint32_t textureId{ 0 };
        float scale{ 1.0f };

        //-------------------------------------------------
        // Sun position
        //-------------------------------------------------

        [[nodiscard]] glm::vec3 GetWorldPosition(const glm::vec3& t_cameraPosition) const
        {
            auto sunPosition{ normalize(direction) };
            sunPosition = -sunPosition;
            sunPosition *= SUN_DISTANCE;

            return t_cameraPosition + sunPosition;
        }
    };
}
