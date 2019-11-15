// This file is part of the SgOgl package.
// 
// Filename: ParticleEmitter.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtx/norm.hpp>
#include "Core.h"
#include "Application.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "Log.h"
#include "Window.h"
#include "OpenGl.h"
#include "buffer/Vao.h"
#include "buffer/BufferLayout.h"
#include "buffer/VertexAttribute.h"
#include "camera/LookAtCamera.h"
#include "scene/Scene.h"
#include "resource/TextureManager.h"
#include "resource/ShaderManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleEmitter::ParticleEmitter(
    scene::Scene* t_scene,
    const size_t t_maxParticles,
    const std::string& t_texturePath,
    const int t_nrOfTextureRows
)
    : m_scene{ t_scene }
    , m_maxParticles{ t_maxParticles }
    , m_vao{ std::make_unique<buffer::Vao>() }
    , m_nrOfTextureRows{ t_nrOfTextureRows }
{
    SG_OGL_CORE_ASSERT(m_scene, "[ParticleEmitter::ParticleEmitter()] Null pointer.")
    SG_OGL_CORE_ASSERT(m_maxParticles > 0, "[ParticleEmitter::ParticleEmitter()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_vao, "[ParticleEmitter::ParticleEmitter()] Null pointer.")

    InitVao();

    // get texture handle for the given texture path
    m_textureId = m_scene->GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(t_texturePath);

    // get a pointer to the shader program
    m_shaderProgram = &m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram("particle_anim");
    SG_OGL_CORE_ASSERT(m_shaderProgram, "[ParticleEmitter::ParticleEmitter()] Null pointer.")
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::particle::ParticleEmitter::ParticleContainer& sg::ogl::particle::ParticleEmitter::GetParticles()
{
    return m_particles;
}

//-------------------------------------------------
// Add
//-------------------------------------------------

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

            //particle.velocity.y += -10 * 0.3f * frametime;
            particle.velocity.y += 10 * 0.5f * frametime;
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
}

void sg::ogl::particle::ParticleEmitter::Render()
{
    PrepareRendering();

    // set uniforms
    m_shaderProgram->SetUniform("projectionMatrix", m_scene->GetApplicationContext()->GetWindow().GetProjectionMatrix());
    m_shaderProgram->SetUniform("numberOfRows", static_cast<float>(m_nrOfTextureRows));
    m_shaderProgram->SetUniform("particleTexture", 0);

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

    // render
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, VERTEX_COUNT, static_cast<int32_t>(m_particles.size()));

    FinishRendering();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::InitVao()
{
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "vPosition" },
    };

    // create a Vbo for the particle vertices
    m_vao->AddVertexDataVbo(
        vertices.data(),
        static_cast<int32_t>(vertices.size()),
        bufferLayout
    );

    // create an empty Vbo for instanced data
    m_vbo = m_vao->AddEmptyVbo(NUMBER_OF_FLOATS_PER_INSTANCE * static_cast<uint32_t>(m_maxParticles));

    // set Vbo attributes
    m_vao->AddInstancedAttribute(m_vbo, 1, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 0);
    m_vao->AddInstancedAttribute(m_vbo, 2, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 4);
    m_vao->AddInstancedAttribute(m_vbo, 3, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 8);
    m_vao->AddInstancedAttribute(m_vbo, 4, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 12);
    m_vao->AddInstancedAttribute(m_vbo, 5, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 16);
    m_vao->AddInstancedAttribute(m_vbo, 6, 1, NUMBER_OF_FLOATS_PER_INSTANCE, 20);
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

void sg::ogl::particle::ParticleEmitter::PrepareRendering() const
{
    m_vao->BindVao();
    m_shaderProgram->Bind();

    // todo -> during Vao creation
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    OpenGl::EnableAlphaBlending();
    OpenGl::DisableWritingIntoDepthBuffer();

    resource::TextureManager::BindForReading(m_textureId, GL_TEXTURE0);
}

void sg::ogl::particle::ParticleEmitter::FinishRendering()
{
    // todo
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);

    OpenGl::EnableWritingIntoDepthBuffer();
    OpenGl::DisableBlending();

    resource::ShaderProgram::Unbind();
    buffer::Vao::UnbindVao();
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
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, NUMBER_OF_FLOATS_PER_INSTANCE * m_maxParticles * sizeof(float), nullptr, GL_STREAM_DRAW);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        NUMBER_OF_FLOATS_PER_INSTANCE * m_particles.size() * sizeof(float),
        &m_instancedData[0]
    );
}
