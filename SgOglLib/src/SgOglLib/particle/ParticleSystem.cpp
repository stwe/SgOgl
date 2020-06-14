#include <glm/gtc/constants.hpp>
#include "ParticleSystem.h"
#include "Random.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleSystem::ParticleSystem()
{
    particlePool.resize(1000);
    Random::Init();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Update(const double t_dt)
{
    const auto dt{ static_cast<float>(t_dt) };

    for (auto& particle : particlePool)
    {
        if (!particle.active)
        {
            continue;
        }

        if (particle.lifeRemaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.lifeRemaining -= dt;

        particle.position += particle.velocity * dt;
        particle.rotation += 0.01f * dt;
    }
}

//-------------------------------------------------
// Emitter
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Emit(const ParticleProperties& t_particleProperties)
{
    auto& particle{ particlePool[m_poolIndex] };
    particle.active = true;
    particle.position = t_particleProperties.position;
    particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

    // Velocity
    particle.velocity = t_particleProperties.velocity;
    particle.velocity.x += t_particleProperties.velocityVariation.x * (Random::Float() - 0.5f);
    particle.velocity.y += t_particleProperties.velocityVariation.y * (Random::Float() - 0.5f);
    particle.velocity.z += t_particleProperties.velocityVariation.z * (Random::Float() - 0.5f);

    // Color
    particle.colorBegin = t_particleProperties.colorBegin;
    particle.colorEnd = t_particleProperties.colorEnd;

    particle.lifeTime = t_particleProperties.lifeTime;
    particle.lifeRemaining = t_particleProperties.lifeTime;
    particle.sizeBegin = t_particleProperties.sizeBegin + t_particleProperties.sizeVariation * (Random::Float() - 0.5f);
    particle.sizeEnd = t_particleProperties.sizeEnd;

    m_poolIndex = --m_poolIndex % particlePool.size();
}
