#pragma once

#include <vector>
#include "Particle.h"

namespace sg::ogl::particle
{
    class ParticleSystem
    {
    public:
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

        void Emit(const ParticleProperties& t_particleProperties);

    protected:

    private:
        uint32_t m_poolIndex{ 999 };
    };
}
