// This file is part of the SgOgl package.
// 
// Filename: Particle.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace sg::ogl::particle
{
    struct Particle
    {
        /**
         * @brief The position in our world.
         */
        glm::vec3 position{ glm::vec3(0.0f) };

        /**
         * @brief The speed (= length of vec) and the direction.
         */
        glm::vec3 velocity{ glm::vec3(0.0f) };

        /**
         * @brief Determine how much the Particle is affected by gravity.
         *        (value 1.0f = affected normal by gravity; less then one means the Particle is more floaty)
         */
        float gravityEffect{ 1.0f };

        /**
         * @brief Specifies how long the Particle should stay alive.
         */
        float lifeTime{ 1.0f };

        /**
         * @brief The Particle can rotate.
         */
        float rotation{ 0.0f };

        /**
         * @brief The size of the Particle.
         */
        float scale{ 1.0f };

        /**
         * @brief The rest of the life.
         */
        float lifeRemaining{ 1.0f };

        /**
         * @brief Indicates whether the Particle is still alive.
         */
        bool active{ false };

        /**
         * @brief The current texture stage in the texture atlas.
         */
        int currentTextureIndex{ 0 };

        /**
         * @brief The next texture stage in the texture atlas.
         */
        int nextTextureIndex{ 0 };

        /**
         * @brief The blend factor between textures.
         */
        float blendFactor{ 0.0f };
    };
}
