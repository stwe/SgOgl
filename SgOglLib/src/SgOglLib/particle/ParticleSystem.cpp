// This file is part of the SgOgl package.
// 
// Filename: ParticleSystem.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "ParticleSystem.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleSystem::ParticleSystem()
{
    particlePool.resize(1000);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Update(const double t_dt)
{
    const auto dt{ static_cast<float>(t_dt) };

    for (auto& particle : particlePool)
    {
        // skip inactive particles
        if (!particle.active)
        {
            continue;
        }

        // if there is no lifetime left, set the particle to inactive
        if (particle.lifeRemaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        // update particle
        particle.lifeRemaining -= dt;
        particle.velocity.y += GRAVITY * particle.gravityEffect * dt;
        particle.position += particle.velocity * dt;
    }
}

//-------------------------------------------------
// Emitter
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Emit(const ParticleRoot& t_particleRoot)
{
    auto& particle{ particlePool[m_poolIndex] };

    particle.position = t_particleRoot.position;
    particle.velocity = t_particleRoot.velocity;
    particle.gravityEffect = t_particleRoot.gravityEffect;
    particle.lifeTime = t_particleRoot.lifeTime;
    particle.rotation = t_particleRoot.rotation;
    particle.scale = t_particleRoot.scale;

    particle.lifeRemaining = t_particleRoot.lifeTime;
    particle.active = true;

    /*
    particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

    particle.velocity = t_particleProperties.velocity;
    particle.velocity.x += t_particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
    particle.velocity.y += t_particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
    particle.velocity.z += t_particleProperties.velocityVariation.z * (Random::Float() - 0.5f);
    */

    m_poolIndex = --m_poolIndex % particlePool.size();
}
