#include "Particle.h"
#include "ParticleEmitter.h"
#include "Log.h"
#include "Window.h"
#include "OpenGl.h"
#include "scene/Scene.h"
#include "camera/LookAtCamera.h"

sg::ogl::particle::Particle::Particle(ParticleEmitter* t_particleEmitter)
    : m_particleEmitter{ t_particleEmitter }
{
    SG_OGL_CORE_ASSERT(m_particleEmitter, "[Particle::Particle()] Null pointer.")

    //SG_OGL_LOG_DEBUG("[Particle::Particle()] A particle was born. Lifetime: {}.", lifetime);

    life = true;
}

void sg::ogl::particle::Particle::Update()
{
    if (lifetime < 0.0f)
    {
        //SG_OGL_LOG_DEBUG("[Particle::Update()] A particle was mark as died.");

        life = false;

        return;
    }

    // particle is alive, thus update
    lifetime -= ParticleEmitter::FRAME_TIME;
    position += velocity * ParticleEmitter::FRAME_TIME;
    color.a -= ParticleEmitter::FRAME_TIME * 2.5f;
}

void sg::ogl::particle::Particle::Render() const
{
    SG_OGL_CORE_ASSERT(life, "[Particle::Render()] A dead particle should not be rendered.")

    // get shader
    auto& shader{ m_particleEmitter->GetParentScene()->GetApplicationContext()->GetShaderManager()->GetShaderProgram("particle") };

    // bind shader
    shader.Bind();

    // get projection matrix
    const auto projectionMatrix{ m_particleEmitter->GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

    // get view matrix
    const auto viewMatrix{ m_particleEmitter->GetParentScene()->GetCurrentCamera().GetViewMatrix() };

    // create model matrix
    auto modelMatrix = translate(glm::mat4(1.0f), position);
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[1][1] = viewMatrix[1][1];
    modelMatrix[1][2] = viewMatrix[2][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[2][1] = viewMatrix[1][2];
    modelMatrix[2][2] = viewMatrix[2][2];

    rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::scale(modelMatrix, glm::vec3(scale));

    // set uniforms
    shader.SetUniform("projectionMatrix", projectionMatrix);
    shader.SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
    shader.SetUniform("color", color);
    shader.SetUniform("particleTexture", 0);

    // bind texture
    resource::TextureManager::BindForReading(m_particleEmitter->GetTextureId(), GL_TEXTURE0);

    // render
    m_particleEmitter->GetVao()->BindVao();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    buffer::Vao::UnbindVao();

    // unbind shader
    resource::ShaderProgram::Unbind();
}
