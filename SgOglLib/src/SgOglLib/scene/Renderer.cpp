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
#include "light/PointLight.h"

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
        // todo verschiedene Shader

        // get ShaderProgram
        auto& shaderProgram{ m_shaderManager.GetShaderProgram("light") };

        // bind ShaderProgram
        shaderProgram->Bind();

        // set model
        shaderProgram->SetUniform("model", t_node.GetWorldMatrix());

        // set view
        shaderProgram->SetUniform("view", m_parentScene->GetCurrentCamera().GetViewMatrix());

        // set projection
        shaderProgram->SetUniform("projection", m_projectionMatrix);

        // set view position
        shaderProgram->SetUniform("viewPos", m_parentScene->GetCurrentCamera().GetPosition());

        // get Material
        auto& material{ *t_node.material };

        material.ns = 32.0;

        // set diffuse map
        shaderProgram->SetUniform("diffuseMap", 0);
        resource::TextureManager::BindForReading(material.mapKd, GL_TEXTURE0);

        // set material
        shaderProgram->SetUniform("material", material);

        // set point light
        light::PointLight light;
        light.position = glm::vec3(0.0f);
        light.ambientIntensity = material.newmtl == "sunMaterial" ? glm::vec3(0.9f) : glm::vec3(0.1f);
        light.diffuseIntensity = glm::vec3(1.0f);
        light.specularIntensity = glm::vec3(1.0f);
        light.linear = 0.0014f;
        light.quadratic = 0.000007f;
        shaderProgram->SetUniform("pointLight", light);

        // render Mesh
        t_node.mesh->InitDraw();
        t_node.mesh->DrawPrimitives();
        t_node.mesh->EndDraw();

        shaderProgram->Unbind();
    }

    for (auto* child : t_node.GetChildren())
    {
        Render(*child);
    }
}
