// This file is part of the SgOgl package.
// 
// Filename: ParticleSystem.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include "ParticleSystem.h"
#include "Random.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleSystem::ParticleSystem(const uint32_t t_textureId)
    : m_textureId{ t_textureId }
{
    Init();
}

sg::ogl::particle::ParticleSystem::ParticleSystem(
    const uint32_t t_textureId,
    const float t_particlesPerSecond,
    const float t_speed,
    const float t_gravityEffect,
    const float t_lifeTime,
    const float t_maxScale
)
    : m_textureId{ t_textureId }
    , m_particlesPerSecond{ t_particlesPerSecond }
    , m_speed{ t_speed }
    , m_gravityEffect{ t_gravityEffect }
    , m_lifeTime{ t_lifeTime }
    , m_maxScale{ t_maxScale }
{
    Init();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::particle::ParticleSystem::GetTextureId() const
{
    return m_textureId;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::SetParticlesPerSecond(const float t_particlesPerSecond)
{
    m_particlesPerSecond = t_particlesPerSecond;
}

void sg::ogl::particle::ParticleSystem::SetSpeed(const float t_speed)
{
    m_speed = t_speed;
}

void sg::ogl::particle::ParticleSystem::SetGravityEffect(const float t_gravityEffect)
{
    m_gravityEffect = t_gravityEffect;
}

void sg::ogl::particle::ParticleSystem::SetLifeTime(const float t_lifeTime)
{
    m_lifeTime = t_lifeTime;
}

void sg::ogl::particle::ParticleSystem::SetMaxScale(const float t_maxScale)
{
    m_maxScale = t_maxScale;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::GenerateParticles(const double t_dt, const glm::vec3& t_systemCenter)
{
    const auto dt{ static_cast<float>(t_dt) };
    const auto particlesToCreate{ m_particlesPerSecond * dt };
    const auto count{ static_cast<int>(floorf(particlesToCreate)) };
    const auto partialParticle{ fmod(particlesToCreate, 1.0f) };

    for (auto i{ 0 }; i < count; ++i)
    {
        Emit(t_systemCenter);
    }

    if (Random::Float() < partialParticle)
    {
        Emit(t_systemCenter);
    }
}

void sg::ogl::particle::ParticleSystem::Update(const double t_dt)
{
    const auto dt{ static_cast<float>(t_dt) };

    for (auto& particle : particles)
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

        const auto life{ particle.lifeRemaining / particle.lifeTime };
        particle.scale = glm::lerp(0.0f, m_maxScale, life);
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Init()
{
    particles.resize(NR_OF_ELEMENTS);
}

//-------------------------------------------------
// Emit
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Emit(const glm::vec3& t_systemCenter)
{
    auto& particle{ particles[m_containerIndex] };

    particle.position = t_systemCenter;

    const auto dirX{ Random::Float() * 2.0f - 1.0f };
    const auto dirZ{ Random::Float() * 2.0f - 1.0f };

    particle.velocity = glm::vec3(dirX, 1.0f, dirZ);
    particle.velocity = normalize(particle.velocity);
    particle.velocity *= m_speed;

    particle.gravityEffect = m_gravityEffect;
    particle.lifeTime = m_lifeTime;
    particle.rotation = 0.0f;
    particle.scale = m_maxScale;

    particle.lifeRemaining = particle.lifeTime;
    particle.active = true;

    m_containerIndex = --m_containerIndex % particles.size();
}
