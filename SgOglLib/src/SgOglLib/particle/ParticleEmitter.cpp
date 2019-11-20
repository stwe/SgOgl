// This file is part of the SgOgl package.
// 
// Filename: ParticleEmitter.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtx/norm.hpp>
#include <random>
#include "Core.h"
#include "Application.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "Log.h"
#include "Window.h"
#include "OpenGl.h"
#include "camera/LookAtCamera.h"
#include "scene/Scene.h"
#include "resource/TextureManager.h"
#include "resource/ShaderManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleEmitter::ParticleEmitter(
    scene::Scene* t_scene,
    const glm::vec3& t_rootPosition,
    const size_t t_maxParticles,
    const size_t t_newParticles,
    const std::string& t_texturePath,
    const int t_nrOfTextureRows
)
    : m_scene{ t_scene }
    , m_rootPosition{ t_rootPosition }
    , m_maxParticles{ t_maxParticles }
    , m_newParticles{ t_newParticles }
    , m_nrOfTextureRows{ t_nrOfTextureRows }
{
    SG_OGL_CORE_ASSERT(m_scene, "[ParticleEmitter::ParticleEmitter()] Null pointer.")
    SG_OGL_CORE_ASSERT(m_maxParticles > 0, "[ParticleEmitter::ParticleEmitter()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_newParticles > 0, "[ParticleEmitter::ParticleEmitter()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_nrOfTextureRows > 0, "[ParticleEmitter::ParticleEmitter()] Invalid value.")

    m_textureId = m_scene->GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(t_texturePath);

    BuildNewParticles();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

size_t sg::ogl::particle::ParticleEmitter::GetMaxParticles() const
{
    return m_maxParticles;
}

sg::ogl::particle::ParticleEmitter::ParticleContainer& sg::ogl::particle::ParticleEmitter::GetParticles()
{
    return m_particles;
}

int sg::ogl::particle::ParticleEmitter::GetNumberOfTextureRows() const
{
    return m_nrOfTextureRows;
}

uint32_t sg::ogl::particle::ParticleEmitter::GetTextureId() const
{
    return m_textureId;
}

uint32_t sg::ogl::particle::ParticleEmitter::GetVboId() const
{
    return m_vboId;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::SetVboId(const uint32_t t_vboId)
{
    m_vboId = t_vboId;
}

void sg::ogl::particle::ParticleEmitter::SetGravity(const float t_gravity)
{
    m_gravity = t_gravity;
}

void sg::ogl::particle::ParticleEmitter::SetGravityEffect(const float t_gravityEffect)
{
    m_gravityEffect = t_gravityEffect;
}

void sg::ogl::particle::ParticleEmitter::SetBuildConfig(const BuildConfig t_buildConfig)
{
    m_buildConfig = t_buildConfig;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::Update(const double t_dt)
{
    // get current camera position
    const auto& cameraPosition{ m_scene->GetCurrentCamera().GetPosition() };

    // cast only once
    const auto frametime{ static_cast<float>(t_dt) };

    // update particles
    for (auto& particle : m_particles)
    {
        // check if the lifetime of the particle has expired
        if (particle.remainingLifetime < 0.0f)
        {
            // mark as dead - the particle is removed below by the EraseRemoveIf function
            particle.life = false;
        }
        // particle is alive, thus update
        else
        {
            SG_OGL_CORE_ASSERT(particle.life, "[ParticleEmitter::Update()] Trying to update a dead particle.")

            particle.remainingLifetime -= frametime;

            // the lifetime can be negative at this point
            if (particle.remainingLifetime < 0.0f)
            {
                particle.cameraDistance = -1.0f;
            }
            else
            {
                particle.cameraDistance = length2(cameraPosition - particle.position);
            }

            particle.velocity.y += m_gravity * m_gravityEffect * frametime;
            particle.position += particle.velocity * frametime;

            // update texture info
            UpdateTextureInfo(particle);
        }
    }

    // remove dead particles
    EraseRemoveIf(m_particles, [](const Particle& t_particle)
        {
            return !t_particle.life;
        }
    );

    // sort particles
    std::sort(m_particles.begin(), m_particles.end());

    BuildNewParticles();
}

void sg::ogl::particle::ParticleEmitter::Render()
{
    // get view matrix
    const auto viewMatrix{ m_scene->GetCurrentCamera().GetViewMatrix() };

    // init counter
    auto counter{ 0 };

    // resize float buffer
    m_instancedData.resize(m_particles.size() * NUMBER_OF_FLOATS_PER_INSTANCE);

    // set instanced data
    for (const auto& particle : m_particles)
    {
        SG_OGL_CORE_ASSERT(particle.life, "[ParticleEmitter::Render()] The particle should be alive.")

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
        m_instancedData[counter++] = GetTextureOffset(particle.textureIndex).x;
        m_instancedData[counter++] = GetTextureOffset(particle.textureIndex).y;
        m_instancedData[counter++] = GetTextureOffset(particle.nextTextureIndex).x;
        m_instancedData[counter++] = GetTextureOffset(particle.nextTextureIndex).y;

        // fill buffer - blend factor
        m_instancedData[counter++] = particle.blendFactor;
    }

    // update vbo
    UpdateVbo();
}

//-------------------------------------------------
// Add
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::BuildNewParticles()
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> velocityX(m_buildConfig.xRange.x, m_buildConfig.xRange.y);
    const std::uniform_real_distribution<float> velocityY(m_buildConfig.yRange.x, m_buildConfig.yRange.y);
    const std::uniform_real_distribution<float> velocityZ(m_buildConfig.zRange.x, m_buildConfig.zRange.y);
    const std::uniform_real_distribution<float> scale(m_buildConfig.scaleRange.x, m_buildConfig.scaleRange.y);
    const std::uniform_real_distribution<float> lifetime(m_buildConfig.lifetimeRange.x, m_buildConfig.lifetimeRange.y);

    const auto currentNrOfParticles{ m_particles.size() };

    if (currentNrOfParticles < m_maxParticles)
    {
        auto newParticles{ m_maxParticles - currentNrOfParticles };
        if (newParticles > m_newParticles)
        {
            newParticles = m_newParticles;
        }

        for (auto i{ 0u }; i < newParticles; ++i)
        {
            Particle particle;

            particle.position = m_rootPosition;
            particle.velocity = glm::vec3(velocityX(engine), velocityY(engine), velocityZ(engine));
            particle.scale = scale(engine);
            particle.lifetime = lifetime(engine);

            AddParticle(particle);
        }
    }
}

bool sg::ogl::particle::ParticleEmitter::AddParticle(Particle& t_particle)
{
    if (m_particles.size() < m_maxParticles)
    {
        // make sure that the remaining lifetime has a valid value
        t_particle.remainingLifetime = t_particle.lifetime;

        /*
        t_particle.cameraDistance = length2(m_scene->GetCurrentCamera().GetPosition() - t_particle.position);
        std::sort(m_particles.begin(), m_particles.end());
        */

        m_particles.push_back(t_particle);

        return true;
    }

    return false;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

glm::vec2 sg::ogl::particle::ParticleEmitter::GetTextureOffset(const int t_textureIndex) const
{
    const auto col{ t_textureIndex % m_nrOfTextureRows };
    const auto row{ t_textureIndex / m_nrOfTextureRows };
    const auto rows{ static_cast<float>(m_nrOfTextureRows) };

    return glm::vec2(col / rows, row / rows);
}

void sg::ogl::particle::ParticleEmitter::UpdateTextureInfo(Particle& t_particle) const
{
    auto lifeFactor{ t_particle.remainingLifetime / t_particle.lifetime };

    // workaround for t_particle.remainingLifetime < 0.0f
    if (lifeFactor < 0.0f)
    {
        lifeFactor = 0.0f;
    }

    SG_OGL_CORE_ASSERT(lifeFactor >= 0.0f && lifeFactor <= 1.0f, "[ParticleEmitter::UpdateTextureInfo()] Invalid value.")

    const auto texturesCount{ m_nrOfTextureRows * m_nrOfTextureRows };
    const auto atlasProgression{ lifeFactor * texturesCount };

    t_particle.textureIndex = static_cast<int>(floor(atlasProgression));
    t_particle.nextTextureIndex = t_particle.textureIndex < texturesCount - 1 ? t_particle.textureIndex + 1 : t_particle.textureIndex;
    t_particle.blendFactor = fmod(atlasProgression, 1.0f);
}

void sg::ogl::particle::ParticleEmitter::UpdateVbo() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, NUMBER_OF_FLOATS_PER_INSTANCE * m_maxParticles * sizeof(float), nullptr, GL_STREAM_DRAW);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        NUMBER_OF_FLOATS_PER_INSTANCE * m_particles.size() * sizeof(float),
        &m_instancedData[0]
    );
}
