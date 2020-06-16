// This file is part of the SgOgl package.
// 
// Filename: ParticleSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <vector>
#include "Particle.h"

namespace sg::ogl::particle
{
    class ParticleSystem
    {
    public:
        using ParticleContainer = std::vector<Particle>;

        /**
         * @brief Number of elements in the particles vector.
         */
        static constexpr uint32_t NR_OF_ELEMENTS{ 1000 };

        static constexpr auto GRAVITY{ -9.81f };

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        ParticleContainer particles;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleSystem();
        ParticleSystem(float t_particlesPerSecond, float t_speed, float t_gravityEffect, float t_lifeTime, float t_maxScale);

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParticlesPerSecond(float t_particlesPerSecond);
        void SetSpeed(float t_speed);
        void SetGravityEffect(float t_gravityEffect);
        void SetLifeTime(float t_lifeTime);
        void SetMaxScale(float t_maxScale);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void GenerateParticles(double t_dt, const glm::vec3& t_systemCenter);

        void Update(double t_dt);

    protected:

    private:
        uint32_t m_containerIndex{ NR_OF_ELEMENTS - 1 };

        float m_particlesPerSecond{ 25.0f };
        float m_speed{ 25.0f };
        float m_gravityEffect{ 0.5f };
        float m_lifeTime{ 4.0f };
        float m_maxScale{ 1.0f };

        //-------------------------------------------------
        // Emit
        //-------------------------------------------------

        void Emit(const glm::vec3& t_systemCenter);
    };
}
