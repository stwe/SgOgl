#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace sg::ogl::particle
{
    struct ParticleProperties
    {
        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };
        glm::vec3 velocityVariation{ glm::vec3(0.0f) };
        glm::vec4 colorBegin{ glm::vec4(1.0f) };
        glm::vec4 colorEnd{ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
        float sizeBegin{ 1.0f };
        float sizeEnd{ 0.0f };
        float sizeVariation{ 0.0f };
        float lifeTime{ 1.0f };
    };

    struct Particle
    {
        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };
        glm::vec4 colorBegin{ glm::vec4(1.0f) };
        glm::vec4 colorEnd{ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
        float rotation{ 0.0f };
        float sizeBegin{ 1.0f };
        float sizeEnd{ 0.0f };

        float lifeTime{ 1.0f };
        float lifeRemaining{ 0.0f };

        bool active{ false };
    };
}
