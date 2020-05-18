// This file is part of the SgOgl package.
// 
// Filename: DirectionalLight.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>

namespace sg::ogl::light
{
    struct DirectionalLight
    {
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        DirectionalLight() = default;

        DirectionalLight(
            const glm::vec3& t_direction,
            const glm::vec3& t_diffuseIntensity,
            const glm::vec3& t_specularIntensity
        )
            : direction{ t_direction }
            , diffuseIntensity{ t_diffuseIntensity }
            , specularIntensity{ t_specularIntensity }
        {}

        DirectionalLight(const DirectionalLight& t_other) = default;
        DirectionalLight(DirectionalLight&& t_other) noexcept = default;
        DirectionalLight& operator=(const DirectionalLight& t_other) = default;
        DirectionalLight& operator=(DirectionalLight&& t_other) noexcept = default;

        virtual ~DirectionalLight() noexcept = default;

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * @brief Direction as a direction from the light source.
         */
        glm::vec3 direction{ glm::vec3(0.0f, -1.0f, 0.0f) };

        glm::vec3 diffuseIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 specularIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
    };
}
