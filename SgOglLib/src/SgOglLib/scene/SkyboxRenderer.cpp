#include "SkyboxRenderer.h"
#include "OpenGl.h"
#include "scene/Scene.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::SkyboxRenderer::SkyboxRenderer(
    resource::ShaderManager& t_shaderManager,
    glm::mat4& t_projection
)
    : BaseRenderer(t_shaderManager, t_projection)
{
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::ogl::scene::SkyboxRenderer::Render(const uint32_t t_textureId, resource::Mesh& t_mesh, const std::string& t_shaderProgramName) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // remove translation from the view matrix
    const auto skyboxViewMatrix{ glm::mat4(glm::mat3(m_parentScene->GetCurrentCamera().GetViewMatrix())) };

    // set shader uniforms
    shaderProgram->SetUniform("projectionMatrix", m_projectionMatrix);
    shaderProgram->SetUniform("viewMatrix", skyboxViewMatrix);
    shaderProgram->SetUniform("cubeSampler", 0);

    // bind cubemap
    resource::TextureManager::BindForReading(t_textureId, GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);

    // bind vao to render mesh
    t_mesh.InitDraw();

    // save old depth func mode
    int32_t oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

    // change depth func
    glDepthFunc(GL_LEQUAL);

    // render mesh
    t_mesh.DrawPrimitives();
    resource::Mesh::EndDraw();

    // restore old depth func mode
    glDepthFunc(oldDepthFuncMode);

    // unbind shader
    shaderProgram->Unbind();
}
