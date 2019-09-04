#include "Renderer.h"
#include "OpenGl.h"
#include "Node.h"
#include "Scene.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Renderer::Renderer(
    resource::ShaderManager& t_shaderManager,
    glm::mat4& t_projection
)
    : BaseRenderer(t_shaderManager, t_projection)
{
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::ogl::scene::Renderer::Render(Node& t_node) const
{
    SG_OGL_CORE_ASSERT(m_parentScene, "[Renderer::Render()] Null pointer.")

    if (t_node.mesh && t_node.material)
    {
        if (m_parentScene->IsPointLight() && t_node.material->newmtl != "sunMaterial")
        {
            // get shader
            auto& shaderProgram{ m_shaderManager.GetShaderProgram("light") };

            // bind shader
            shaderProgram->Bind();

            // set model matrix
            shaderProgram->SetUniform("model", t_node.GetWorldMatrix());

            // set view matrix
            shaderProgram->SetUniform("view", m_parentScene->GetCurrentCamera().GetViewMatrix());

            // set projection matrix
            shaderProgram->SetUniform("projection", m_projectionMatrix);

            // set view (camera) position
            shaderProgram->SetUniform("viewPos", m_parentScene->GetCurrentCamera().GetPosition());

            // get material
            auto& material{ *t_node.material };

            // set and bind diffuse map
            shaderProgram->SetUniform("diffuseMap", 0);
            resource::TextureManager::BindForReading(material.mapKd, GL_TEXTURE0);

            // set material
            shaderProgram->SetUniform("material", material);

            // set point light
            shaderProgram->SetUniform("pointLight", m_parentScene->GetPointLight());

            // render Mesh
            t_node.mesh->InitDraw();
            t_node.mesh->DrawPrimitives();
            t_node.mesh->EndDraw();

            // unbind shader
            shaderProgram->Unbind();
        }
        else
        {
            // get shader
            auto& shaderProgram{ m_shaderManager.GetShaderProgram("model") };

            // bind shader
            shaderProgram->Bind();

            // calc mvp matrix
            const auto mvp{ m_projectionMatrix * m_parentScene->GetCurrentCamera().GetViewMatrix() * t_node.GetWorldMatrix() };
            shaderProgram->SetUniform("transform", mvp);

            // set ambient intensity
            shaderProgram->SetUniform("ambientIntensity", glm::vec3(0.9f));

            // get material
            auto& material{ t_node.material };

            // set and bind diffuse map
            shaderProgram->SetUniform("diffuseMap", 0);
            resource::TextureManager::BindForReading(material->mapKd, GL_TEXTURE0);

            // render mesh
            t_node.mesh->InitDraw();
            t_node.mesh->DrawPrimitives();
            t_node.mesh->EndDraw();

            // unbind shader
            shaderProgram->Unbind();
        }
    }

    for (auto* child : t_node.GetChildren())
    {
        Render(*child);
    }
}
