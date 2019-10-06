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

    m_textureId = m_scene->GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath(t_texturePath);
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
        m_particles.push_back(t_particle);

        return true;
    }

    //SG_OGL_LOG_DEBUG("[ParticleEmitter::AddParticle()] A particle could not be added.");

    return false;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

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

    // debug output
    if (m_particles.empty() && !m_emptyContainer)
    {
        m_emptyContainer = true;
        SG_OGL_LOG_DEBUG("[ParticleEmitter::Update()] The particles container is empty.");
    }
}

void sg::ogl::particle::ParticleEmitter::Render()
{
    PrepareRendering();

    // render particles
    for (auto& particle : m_particles)
    {
        SG_OGL_CORE_ASSERT(particle.life, "[Particle::Render()] A dead particle should not be rendered.")

        // get projection matrix
        const auto projectionMatrix{ m_scene->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // get view matrix
        const auto viewMatrix{ m_scene->GetCurrentCamera().GetViewMatrix() };

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
        scale(modelMatrix, glm::vec3(particle.scale));

        // set uniforms
        m_shaderProgram->SetUniform("projectionMatrix", projectionMatrix);
        m_shaderProgram->SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
        m_shaderProgram->SetUniform("color", particle.color);
        m_shaderProgram->SetUniform("particleTexture", 0);
        m_shaderProgram->SetUniform("numberOfRows", static_cast<float>(m_nrOfTextureRows));
        m_shaderProgram->SetUniform("offset", GetTextureOffset(particle.textureIndex));

        // render
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    FinishRendering();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::InitVao() const
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

    // todo

    // create an empty Vbo
    //const auto vbo{ m_vao->AllocateMemory(NUMBER_OF_FLOATS_PER_INSTANCE * MAX_INSTANCES) };

    /*   4     4     4     4     4       1       = 21 Floats
     * [ColA][ColB][ColC][ColD][TexOff][Blend]
     */

     // set Vbo attributes                                                     21 x 4 byte = 84 bytes
    /*
    m_vao->AddInstancedAttribute(vbo, 1, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 0);
    m_vao->AddInstancedAttribute(vbo, 2, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 4);
    m_vao->AddInstancedAttribute(vbo, 3, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 8);
    m_vao->AddInstancedAttribute(vbo, 4, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 12);
    m_vao->AddInstancedAttribute(vbo, 5, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 16);
    m_vao->AddInstancedAttribute(vbo, 6, 1, NUMBER_OF_FLOATS_PER_INSTANCE, 20); // 20 x 4 = offset 80
    */
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

glm::vec2 sg::ogl::particle::ParticleEmitter::GetTextureOffset(const int t_textureIndex) const
{
    const auto col{ t_textureIndex % m_nrOfTextureRows };
    const auto row{ t_textureIndex / m_nrOfTextureRows };

    // only need to cast one integer to float
    const auto rows{ static_cast<float>(m_nrOfTextureRows) };

    return glm::vec2(col / rows, row / rows);
}

void sg::ogl::particle::ParticleEmitter::PrepareRendering()
{
    m_vao->BindVao();

    m_shaderProgram = &m_scene->GetApplicationContext()->GetShaderManager()->GetShaderProgram("particle");
    SG_OGL_CORE_ASSERT(m_shaderProgram, "[ParticleEmitter::PrepareRendering()] Null pointer.")
    m_shaderProgram->Bind();

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(false);

    resource::TextureManager::BindForReading(m_textureId, GL_TEXTURE0);
}

void sg::ogl::particle::ParticleEmitter::FinishRendering()
{
    buffer::Vao::UnbindVao();
    resource::ShaderProgram::Unbind();

    glDepthMask(true);
    glDisable(GL_BLEND);
}
