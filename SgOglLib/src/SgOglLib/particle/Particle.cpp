#include "Particle.h"
#include "ParticleGenerator.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::Particle::Particle(ParticleGenerator* t_parentParticleGenerator)
    : m_parentParticleGenerator{ t_parentParticleGenerator }
{
    m_parentParticleGenerator->AddParticle(*this);
}

sg::ogl::particle::Particle::Particle(
    ParticleGenerator* t_parentParticleGenerator,
    const glm::vec3& t_position,
    const glm::vec3& t_velocity,
    const float t_gravity,
    const float t_lifeLength,
    const float t_rotation,
    const float t_scale
)
    : m_parentParticleGenerator{ t_parentParticleGenerator }
    , position{ t_position }
    , velocity{ t_velocity }
    , gravity{ t_gravity }
    , lifeLength{ t_lifeLength }
    , rotation{ t_rotation }
    , scale{ t_scale }
{
    m_parentParticleGenerator->AddParticle(*this);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool sg::ogl::particle::Particle::Update(const double t_dt)
{
    velocity.y += -50.0f * gravity * static_cast<float>(t_dt);
    position += velocity * static_cast<float>(t_dt);

    m_elapsedTime += static_cast<float>(t_dt);

    return m_elapsedTime < lifeLength;
}
