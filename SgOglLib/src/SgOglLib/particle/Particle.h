#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "ParticleEmitter.h"

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
        bool life{ true };

        int textureIndex{ 0 };

        void Update()
        {
            if (lifetime < 0.0f)
            {
                //SG_OGL_LOG_DEBUG("[Particle::Update()] A particle was mark as died.");

                life = false;

                return;
            }

            // particle is alive, thus update
            lifetime -= ParticleEmitter::FRAME_TIME;
            position += velocity * ParticleEmitter::FRAME_TIME;
            color.a -= ParticleEmitter::FRAME_TIME;
        }
    };
}
