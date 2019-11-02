#pragma once

#include <glm/vec3.hpp>

namespace sg::ogl::light
{
    struct DirectionalLight
    {
        static constexpr glm::vec3 DAWN{ glm::vec3(-1.0f, 0.0f, 0.0f) };
        static constexpr glm::vec3 MID_DAY{ glm::vec3(0.0f, 1.0f, 0.0f) };
        static constexpr glm::vec3 DUSK{ glm::vec3(1.0f, 0.0f, 0.0f) };

        glm::vec3 direction{ MID_DAY };
        glm::vec3 diffuseIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 specularIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
    };
}
