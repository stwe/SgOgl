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
    struct ParticleRoot
    {
        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };
        float gravityEffect{ 1.0f };
        float lifeTime{ 1.0f };
        float rotation{ 0.0f };
        float scale{ 1.0f };
    };

    class ParticleSystem
    {
    public:
        static constexpr auto GRAVITY{ -9.81f };

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        std::vector<Particle> particlePool;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleSystem();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(double t_dt);

        //-------------------------------------------------
        // Emitter
        //-------------------------------------------------

        void Emit(const ParticleRoot& t_particleRoot);

    protected:

    private:
        uint32_t m_poolIndex{ 999 };
    };
}
