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

        float rotation{ 0.0f };
        float scale{ 1.0f };

        float lifetime{ 5.0f };
        float remainingLifetime{ 5.0f };
        bool life{ true };

        int textureIndex{ 0 };
        int nextTextureIndex{ 0 };
        float blendFactor{ 0.0f };
    };
}
