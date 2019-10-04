#include "ParticleEmitter.h"
#include "Particle.h"
#include "Log.h"
#include "buffer/Vao.h"
#include "buffer/BufferLayout.h"
#include "buffer/VertexAttribute.h"
#include "scene/Scene.h"

sg::ogl::particle::ParticleEmitter::ParticleEmitter(scene::Scene* t_scene, const size_t t_maxParticles)
    : m_scene{ t_scene }
    , m_maxParticles{ t_maxParticles }
    , m_vao{ std::make_unique<buffer::Vao>() }
{
    SG_OGL_CORE_ASSERT(m_scene, "[ParticleEmitter::ParticleEmitter()] Null pointer.")
    SG_OGL_CORE_ASSERT(m_maxParticles > 0, "[ParticleEmitter::ParticleEmitter()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_vao, "[ParticleEmitter::ParticleEmitter()] Null pointer.")

    Init();
}

sg::ogl::scene::Scene* sg::ogl::particle::ParticleEmitter::GetParentScene() const
{
    return m_scene;
}

sg::ogl::particle::ParticleEmitter::VaoUniquePtr& sg::ogl::particle::ParticleEmitter::GetVao()
{
    return m_vao;
}

sg::ogl::particle::ParticleEmitter::ParticleContainer& sg::ogl::particle::ParticleEmitter::GetParticles()
{
    return m_particles;
}

bool sg::ogl::particle::ParticleEmitter::AddParticle(Particle& t_particle)
{
    if (m_particles.size() < m_maxParticles)
    {
        m_particles.push_back(t_particle);

        return true;
    }

    //SG_OGL_LOG_DEBUG("[ParticleEmitter::AddParticle()] A particle could not be added.");

    return false;
}

void sg::ogl::particle::ParticleEmitter::Update()
{
    // update particles
    for (auto& particle : m_particles)
    {
        particle.Update();
    }

    // remove dead particles
    EraseRemoveIf(m_particles, [](const Particle& t_particle)
        {
            return !t_particle.life;
        }
    );

    if (m_particles.empty() && !m_emptyContainer)
    {
        m_emptyContainer = true;
        SG_OGL_LOG_DEBUG("[ParticleEmitter::Update()] The particles container is empty.");
    }
}

void sg::ogl::particle::ParticleEmitter::Render()
{
    // render particles
    for (auto& particle : m_particles)
    {
        particle.Render();
    }
}

void sg::ogl::particle::ParticleEmitter::Init()
{
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "vPosition" },
    };

    m_vao->AllocateVertices(
        vertices.data(),
        static_cast<int32_t>(vertices.size()),
        static_cast<uint32_t>(vertices.size()) * sizeof(float),
        bufferLayout
    );

    m_textureId = m_scene->GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/snow.png");
}
