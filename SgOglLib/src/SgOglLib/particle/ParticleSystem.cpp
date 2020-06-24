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
#include "Core.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "buffer/Vao.h"
#include "buffer/Vbo.h"
#include "resource/Mesh.h"
#include "resource/ModelManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleSystem::ParticleSystem(scene::Scene* t_scene, const uint32_t t_textureId, const int t_textureRows)
    : m_scene{ t_scene }
    , m_textureId{ t_textureId }
    , m_textureRows{ t_textureRows }
{
    SG_OGL_CORE_ASSERT(t_scene, "[ParticleSystem::ParticleSystem()] Null pointer.");
    SG_OGL_CORE_ASSERT(t_textureId, "[ParticleSystem::ParticleSystem()] Invalid value.");
    SG_OGL_CORE_ASSERT(t_textureRows, "[ParticleSystem::ParticleSystem()] Invalid value.");

    Init();
}

sg::ogl::particle::ParticleSystem::ParticleSystem(
    scene::Scene* t_scene,
    const uint32_t t_textureId,
    const int t_textureRows,
    const float t_particlesPerSecond,
    const float t_speed,
    const float t_gravityEffect,
    const float t_lifeTime,
    const float t_maxScale
)
    : m_scene{ t_scene }
    , m_textureId{ t_textureId }
    , m_textureRows{ t_textureRows }
    , m_particlesPerSecond{ t_particlesPerSecond }
    , m_speed{ t_speed }
    , m_gravityEffect{ t_gravityEffect }
    , m_lifeTime{ t_lifeTime }
    , m_maxScale{ t_maxScale }
{
    SG_OGL_CORE_ASSERT(t_scene, "[ParticleSystem::ParticleSystem()] Null pointer.");
    SG_OGL_CORE_ASSERT(t_textureId, "[ParticleSystem::ParticleSystem()] Invalid value.");
    SG_OGL_CORE_ASSERT(t_textureRows, "[ParticleSystem::ParticleSystem()] Invalid value.");

    Init();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::particle::ParticleSystem::GetTextureId() const
{
    return m_textureId;
}

int sg::ogl::particle::ParticleSystem::GetTextureRows() const
{
    return m_textureRows;
}

const sg::ogl::resource::Mesh& sg::ogl::particle::ParticleSystem::GetMesh() const noexcept
{
    return *m_mesh;
}

sg::ogl::resource::Mesh& sg::ogl::particle::ParticleSystem::GetMesh() noexcept
{
    return *m_mesh;
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
        if (particle.lifeRemaining <= t_dt)
        {
            particle.active = false;
            continue;
        }

        // update particle
        particle.lifeRemaining -= dt;
        particle.velocity.y += GRAVITY * particle.gravityEffect * dt;
        particle.position += particle.velocity * dt;

        const auto life{ particle.lifeRemaining / particle.lifeTime };

        SG_OGL_CORE_ASSERT(life >= 0.0f && life <= 1.0f, "[ParticleEmitter::ParticleSystem::Update()] Invalid value:." + std::to_string(life));

        particle.scale = glm::lerp(0.0f, m_maxScale, life);

        const auto atlasProgression{ life * static_cast<float>(m_nrTextures) };

        particle.currentTextureIndex = static_cast<int>(floor(atlasProgression));
        particle.nextTextureIndex = particle.currentTextureIndex < m_nrTextures - 1 ? particle.currentTextureIndex + 1 : particle.currentTextureIndex;
        particle.blendFactor = fmod(atlasProgression, 1.0f);
    }
}

void sg::ogl::particle::ParticleSystem::Render()
{
    // if no instancing is used, there is nothing to do here
    if (!instancing)
    {
        return;
    }

    // get view matrix
    const auto viewMatrix{ m_scene->GetCurrentCamera().GetViewMatrix() };

    // init counter
    auto counter{ 0 };

    // set instanced data
    for (const auto& particle : particles)
    {
        // skip inactive particles
        if (!particle.active)
        {
            continue;
        }

        // create model matrix
        auto modelMatrix = translate(glm::mat4(1.0f), particle.position);
        modelMatrix[0][0] = viewMatrix[0][0];
        modelMatrix[0][1] = viewMatrix[1][0];
        modelMatrix[0][2] = viewMatrix[2][0];
        modelMatrix[1][0] = viewMatrix[0][1];
        modelMatrix[1][1] = viewMatrix[1][1];
        modelMatrix[1][2] = viewMatrix[2][1];
        modelMatrix[2][0] = viewMatrix[0][2];
        modelMatrix[2][1] = viewMatrix[1][2];
        modelMatrix[2][2] = viewMatrix[2][2];

        //rotate(modelMatrix, glm::radians(particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = scale(modelMatrix, glm::vec3(particle.scale));

        // create modelView matrix
        const auto matrix{ viewMatrix * modelMatrix };

        // fill buffer - modelView matrix
        m_instancedData[counter++] = matrix[0][0];
        m_instancedData[counter++] = matrix[0][1];
        m_instancedData[counter++] = matrix[0][2];
        m_instancedData[counter++] = matrix[0][3];

        m_instancedData[counter++] = matrix[1][0];
        m_instancedData[counter++] = matrix[1][1];
        m_instancedData[counter++] = matrix[1][2];
        m_instancedData[counter++] = matrix[1][3];

        m_instancedData[counter++] = matrix[2][0];
        m_instancedData[counter++] = matrix[2][1];
        m_instancedData[counter++] = matrix[2][2];
        m_instancedData[counter++] = matrix[2][3];

        m_instancedData[counter++] = matrix[3][0];
        m_instancedData[counter++] = matrix[3][1];
        m_instancedData[counter++] = matrix[3][2];
        m_instancedData[counter++] = matrix[3][3];

        // fill buffer - texture offsets
        m_instancedData[counter++] = GetTextureOffset(particle.currentTextureIndex).x;
        m_instancedData[counter++] = GetTextureOffset(particle.currentTextureIndex).y;
        m_instancedData[counter++] = GetTextureOffset(particle.nextTextureIndex).x;
        m_instancedData[counter++] = GetTextureOffset(particle.nextTextureIndex).y;

        // fill buffer - blend factor
        m_instancedData[counter++] = particle.blendFactor;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, NUMBER_OF_FLOATS_PER_INSTANCE * particles.size() * sizeof(float), nullptr, GL_STREAM_DRAW);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        NUMBER_OF_FLOATS_PER_INSTANCE * particles.size() * sizeof(float),
        &m_instancedData[0]
    );
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

glm::vec2 sg::ogl::particle::ParticleSystem::GetTextureOffset(const int t_textureIndex) const
{
    const auto col{ t_textureIndex % m_textureRows };
    const auto row{ t_textureIndex / m_textureRows };
    const auto rows{ static_cast<float>(m_textureRows) };

    return glm::vec2(col / rows, row / rows);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::particle::ParticleSystem::Init()
{
    particles.resize(NR_OF_ELEMENTS);
    m_nrTextures = m_textureRows * m_textureRows;

    // the following is only required for instancing ...

    // set the size of container for the instanced data
    m_instancedData.resize(particles.size() * NUMBER_OF_FLOATS_PER_INSTANCE);

    // create Mesh
    m_mesh = std::make_unique<resource::Mesh>();
    SG_OGL_CORE_ASSERT(m_mesh, "[ParticleSystem::Init()] Null pointer.");

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // add Vbo
    m_mesh->GetVao().AddVertexDataVbo(
        resource::ModelManager::GetParticleVertices().data(),
        static_cast<int32_t>(resource::ModelManager::GetParticleVertices().size()) / 2,
        bufferLayout
    );

    // create an additional empty Vbo for instanced data
    m_vboId = buffer::Vbo::GenerateVbo();

    // init empty
    const auto floatCount{ NUMBER_OF_FLOATS_PER_INSTANCE * NR_OF_ELEMENTS };
    buffer::Vbo::InitEmpty(m_vboId, floatCount, GL_STREAM_DRAW);

    // add the empty Vbo to the Mesh
    auto& vao{ m_mesh->GetVao() };
    vao.BindVao();

    // and set the layout
    buffer::Vbo::AddInstancedAttribute(m_vboId, 1, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 0);
    buffer::Vbo::AddInstancedAttribute(m_vboId, 2, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 4);
    buffer::Vbo::AddInstancedAttribute(m_vboId, 3, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 8);
    buffer::Vbo::AddInstancedAttribute(m_vboId, 4, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 12);
    buffer::Vbo::AddInstancedAttribute(m_vboId, 5, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 16);
    buffer::Vbo::AddInstancedAttribute(m_vboId, 6, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 20);

    buffer::Vao::UnbindVao();
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
