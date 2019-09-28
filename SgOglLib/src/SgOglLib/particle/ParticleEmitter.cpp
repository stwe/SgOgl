#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "ParticleEmitter.h"
#include "Particle.h"
#include "OpenGl.h"
#include "Window.h"
#include "scene/Scene.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::particle::ParticleEmitter::ParticleEmitter(scene::Scene* t_scene)
    : m_parentScene{ t_scene }
{
    m_particles.resize(MAX_PARTICLES);

    m_positions = new float[MAX_PARTICLES * 4];
    m_colors = new float[MAX_PARTICLES * 4];

    InitBuffers();
}

sg::ogl::particle::ParticleEmitter::~ParticleEmitter() noexcept
{
    delete[] m_positions;
    delete[] m_colors;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::Update(const double t_dt)
{
    // respawn
    const auto newParticles{ static_cast<int>(t_dt * NEW_PARTICLES) };
    for (auto i{ 0 }; i < newParticles; ++i)
    {
        const auto deadParticleIdx{ GetFirstDeadParticle() };

        // set life and position
        m_particles[deadParticleIdx].life = 3.0f; // This particle will live 3 seconds.
        m_particles[deadParticleIdx].position = glm::vec3(100.0f, 20.0f, 160.0f);

        // set velocity
        const auto spread{ 1.5f };
        auto maindir{ glm::vec3(0.0f, 10.0f, 0.0f) };
        auto randomdir{ glm::vec3(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f
        ) };
        m_particles[deadParticleIdx].velocity = maindir + randomdir * spread;

        // set a random color
        m_particles[deadParticleIdx].color.x = static_cast<float>(rand() % 256);
        m_particles[deadParticleIdx].color.y = static_cast<float>(rand() % 256);
        m_particles[deadParticleIdx].color.z = static_cast<float>(rand() % 256);
        m_particles[deadParticleIdx].color.w = static_cast<float>((rand() % 256) / 3);

        // set size
        m_particles[deadParticleIdx].size = (rand() % 1000) / 2000.0f + 0.1f;
    }

    // simulate all particles
    m_particlesCount = 0;
    for (auto i{ 0 }; i < MAX_PARTICLES; ++i)
    {
        auto& particle{ m_particles[i] };

        if (particle.life > 0.0f)
        {
            // decrease life
            particle.life -= static_cast<float>(t_dt);

            if (particle.life > 0.0f)
            {
                auto& cameraPosition{ m_parentScene->GetCurrentCamera().GetPosition() };

                // Simulate simple physics : gravity only, no collisions
                particle.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * static_cast<float>(t_dt) * 0.5f;
                particle.position += particle.velocity * static_cast<float>(t_dt);
                particle.cameraDistance = length2(particle.position - cameraPosition);

                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer.
                m_positions[4 * m_particlesCount + 0] = particle.position.x;
                m_positions[4 * m_particlesCount + 1] = particle.position.y;
                m_positions[4 * m_particlesCount + 2] = particle.position.z;
                m_positions[4 * m_particlesCount + 3] = particle.size;

                m_colors[4 * m_particlesCount + 0] = particle.color.x;
                m_colors[4 * m_particlesCount + 1] = particle.color.y;
                m_colors[4 * m_particlesCount + 2] = particle.color.z;
                m_colors[4 * m_particlesCount + 3] = particle.color.w;
            }
            else
            {
                // Particles that just died will be put at the end of the buffer in SortParticles().
                particle.cameraDistance = -1.0f;
            }

            m_particlesCount++;
        }
    }

    SortParticles();

    // update buffers

    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particlesCount * sizeof(float) * 4, m_positions);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particlesCount * sizeof(float) * 4, m_colors);

    glBindVertexArray(0);
}

void sg::ogl::particle::ParticleEmitter::Render()
{
    glBindVertexArray(m_vaoId);

    auto& shader{ m_parentScene->GetApplicationContext()->GetShaderManager()->GetShaderProgram("particle") };

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto viewMatrix{ m_parentScene->GetCurrentCamera().GetViewMatrix() };
    auto projectionMatrix{ m_parentScene->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

    shader.Bind();

    shader.SetUniform("cameraRightWorldspace", glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]));
    shader.SetUniform("cameraUpWorldspace", glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]));
    shader.SetUniform("vpMatrix", projectionMatrix * viewMatrix);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : x + y + z + size => 4
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    glVertexAttribPointer(
        2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : r + g + b + a => 4
        GL_UNSIGNED_BYTE,                 // type
        GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    resource::ShaderProgram::Unbind();

    glBindVertexArray(0);
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::particle::ParticleEmitter::InitBuffers()
{
    // create Vao
    glGenVertexArrays(1, &m_vaoId);

    // bind Vao
    glBindVertexArray(m_vaoId);

    /// Vertices

    // create Vbo with vertex data
    glGenBuffers(1, &m_vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_DATA.size() * sizeof(float), VERTEX_BUFFER_DATA.data(), GL_STATIC_DRAW);

    /// Positions

    // create empty Vbo with positions (3x float) and sizes (1x float)
    glGenBuffers(1, &m_vboPositions);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

    /// Colors

    // create empty Vbo with colors (4x float)
    glGenBuffers(1, &m_vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(float), nullptr, GL_STREAM_DRAW);

    /// unbind

    // unbind Vao
    glBindVertexArray(0);
}

int sg::ogl::particle::ParticleEmitter::GetFirstDeadParticle()
{
    for (auto i{ m_idxOfLastDeadParticle }; i < MAX_PARTICLES; ++i)
    {
        if (m_particles[i].life < 0.0f)
        {
            m_idxOfLastDeadParticle = i;

            return i;
        }
    }

    for (auto i{ 0 }; i < m_idxOfLastDeadParticle; ++i)
    {
        if (m_particles[i].life < 0.0f)
        {
            m_idxOfLastDeadParticle = i;

            return i;
        }
    }

    // override the first one

    return 0;
}

void sg::ogl::particle::ParticleEmitter::SortParticles()
{
    std::sort(m_particles.begin(), m_particles.end());
}
