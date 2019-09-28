#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sg::ogl::particle
{
    struct Particle
    {
        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };
        glm::vec4 color{ glm::vec4(1.0f) };

        float size{ 0.0f };

        // if < 0.0f : dead and unused
        float life{ -1.0f };

        // if dead : -1.0f
        float cameraDistance{ -1.0f };

        // std::sort will use operator< as comparison function by default
        bool operator<(const Particle& t_other) const
        {
            // sort in reverse order: far particles drawn first
            return cameraDistance > t_other.cameraDistance;
        }
    };
}
