#include "ParticleGenerator.h"
#include "Particle.h"
#include "scene/Scene.h"
#include "buffer/Vao.h"
#include "camera/LookAtCamera.h"
#include "Window.h"
#include "OpenGl.h"

sg::ogl::particle::ParticleGenerator::ParticleGenerator(scene::Scene* t_scene)
    : m_parentScene{ t_scene }
    , m_vao{ std::make_unique<buffer::Vao>() }
{
    // create an Vbo holding the vertices
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "vPosition" },
    };

    m_vao->AllocateVertices(VERTICES.data(), VERTICES.size(), VERTICES.size() * sizeof(float), bufferLayout);

    // create an empty Vbo
    const auto vbo{ m_vao->AllocateMemory(NUMBER_OF_FLOATS_PER_INSTANCE * MAX_INSTANCES) };

    /*   4     4     4     4     4       1       = 21 Floats
     * [ColA][ColB][ColC][ColD][TexOff][Blend]
     */

    // set Vbo attributes                                                     21 x 4 byte = 84 bytes
    m_vao->AddInstancedAttribute(vbo, 1, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 0);
    m_vao->AddInstancedAttribute(vbo, 2, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 4);
    m_vao->AddInstancedAttribute(vbo, 3, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 8);
    m_vao->AddInstancedAttribute(vbo, 4, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 12);
    m_vao->AddInstancedAttribute(vbo, 5, 4, NUMBER_OF_FLOATS_PER_INSTANCE, 16);
    m_vao->AddInstancedAttribute(vbo, 6, 1, NUMBER_OF_FLOATS_PER_INSTANCE, 20); // 20 x 4 = offset 80
}

void sg::ogl::particle::ParticleGenerator::Update()
{
    if (m_particles.size() > 0)
    {
        SG_OGL_LOG_DEBUG("Particles: {}", m_particles.size());
    }

    auto it{ m_particles.begin() };
    while (it != m_particles.end())
    {
        const auto result{ it->Update(0.016) };
        if (!result)
        {
            it = m_particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// todo

void sg::ogl::particle::ParticleGenerator::Render()
{
    // get shader
    auto& shader{ m_parentScene->GetApplicationContext()->GetShaderManager()->GetShaderProgram("particle") };

    // bind shader
    shader.Bind();

    for (auto& particle : m_particles)
    {
        // get projection matrix
        const auto projectionMatrix{ m_parentScene->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // get view matrix
        const auto viewMatrix{ m_parentScene->GetCurrentCamera().GetViewMatrix() };

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

        rotate(modelMatrix, glm::radians(particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        scale(modelMatrix, glm::vec3(particle.scale));

        // set uniforms
        shader.SetUniform("projectionMatrix", projectionMatrix);
        shader.SetUniform("modelViewMatrix", viewMatrix * modelMatrix);

        // render
        m_vao->BindVao();
        //glDrawArrays(GL_POINTS, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        buffer::Vao::UnbindVao();
    }

    // unbind shader
    resource::ShaderProgram::Unbind();
}

void sg::ogl::particle::ParticleGenerator::AddParticle(Particle& t_particle)
{
    m_particles.push_back(t_particle);
}
