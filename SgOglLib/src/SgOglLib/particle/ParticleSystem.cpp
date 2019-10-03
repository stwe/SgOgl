#include <glm/gtx/norm.hpp>
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "scene/Scene.h"

sg::ogl::particle::ParticleSystem::ParticleSystem(
    scene::Scene* t_scene,
    const float t_pps,
    const float t_speed,
    const float t_gravityComplient,
    const float t_lifeLength
)
    : m_pps{ t_pps }
    , m_speed{ t_speed }
    , m_gravityComplient{ t_gravityComplient }
    , m_lifeLength{ t_lifeLength }
    , m_particleGenerator{ std::make_unique<ParticleGenerator>(t_scene) }
{
}

void sg::ogl::particle::ParticleSystem::GenerateParticles(const glm::vec3& t_systemCenter) const
{
    const auto particlesToCreate{ m_pps * 0.016f };
    const auto count{ static_cast<int>(floor(particlesToCreate)) };
    const auto partialParticle{ fmod(particlesToCreate, 1.0f) };

    for (auto i{ 0 }; i < count; ++i)
    {
        EmitParticle(t_systemCenter);
    }

    if (static_cast<float>(rand()) < partialParticle)
    {
        EmitParticle(t_systemCenter);
    }
}

void sg::ogl::particle::ParticleSystem::Update()
{
    m_particleGenerator->Update();
}

void sg::ogl::particle::ParticleSystem::Render()
{
    m_particleGenerator->Render();
}

void sg::ogl::particle::ParticleSystem::EmitParticle(const glm::vec3& t_center) const
{
    const auto dirX{ static_cast<float>(rand()) * 2.0f - 1.0f };
    const auto dirZ{ static_cast<float>(rand()) * 2.0f - 1.0f };

    auto velocity{ glm::vec3(dirX, 1.0f, dirZ) };
    velocity = normalize(velocity);
    velocity *= m_speed;

    Particle particle{ m_particleGenerator.get(), glm::vec3(t_center), velocity, m_gravityComplient, m_lifeLength, 0, 1 };
}
