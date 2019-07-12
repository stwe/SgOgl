#include "ModelRenderer.h"
#include "OpenGl.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "resource/Model.h"
#include "resource/Material.h"
#include "camera/LookAtCamera.h"
#include "math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::renderer::ModelRenderer::ModelRenderer(
    resource::ShaderManager& t_shaderManager,
    resource::TextureManager& t_textureManager,
    camera::LookAtCamera& t_camera,
    glm::mat4& t_projection
)
    : m_shaderManager{ t_shaderManager }
    , m_textureManager{ t_textureManager }
    , m_camera{ t_camera }
    , m_projectionMatrix{ t_projection }
{
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::ogl::renderer::ModelRenderer::Render(resource::Model& t_model, math::Transform& t_transform, const std::string& t_shaderProgramName) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // set transform
    const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * t_transform.GetModelMatrix() };
    shaderProgram->SetUniform("transform", mvp);

    // set ambient intensity
    shaderProgram->SetUniform("ambientIntensity", glm::vec3(1.0f));

    // for each mesh
    for (const auto& mesh : t_model.GetMeshes())
    {
        // get Material
        auto& material{ mesh->GetMaterial() };

        // set diffuse map
        shaderProgram->SetUniform("diffuseMap", 0);
        resource::TextureManager::BindForReading(material->mapKd, GL_TEXTURE0);

        // render Mesh
        mesh->InitDraw();
        mesh->DrawPrimitives();
        mesh->EndDraw();
    }

    shaderProgram->Unbind();
}

void sg::ogl::renderer::ModelRenderer::RenderNormals(resource::Model& t_model, math::Transform& t_transform, const std::string& t_shaderProgramName) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // set mvp
    shaderProgram->SetUniform("projection", m_projectionMatrix);
    shaderProgram->SetUniform("view", m_camera.GetViewMatrix());
    shaderProgram->SetUniform("model", t_transform.GetModelMatrix());

    // for each mesh
    for (const auto& mesh : t_model.GetMeshes())
    {
        // render Mesh
        mesh->InitDraw();
        mesh->DrawPrimitives();
        mesh->EndDraw();
    }

    shaderProgram->Unbind();
}
