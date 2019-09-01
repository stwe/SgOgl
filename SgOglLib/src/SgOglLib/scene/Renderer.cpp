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
        // todo transparency

        // get ShaderProgram
        auto& shaderProgram{ m_shaderManager.GetShaderProgram("model") };

        // bind ShaderProgram
        shaderProgram->Bind();

        // calc and set mvp
        const auto mvp{ m_projectionMatrix * m_parentScene->GetCurrentCamera().GetViewMatrix() * t_node.GetWorldMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // set ambient intensity
        shaderProgram->SetUniform("ambientIntensity", glm::vec3(1.0f));

        // get Material
        const auto& material{ t_node.material };

        // set diffuse map
        shaderProgram->SetUniform("diffuseMap", 0);
        resource::TextureManager::BindForReading(material->mapKd, GL_TEXTURE0);

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
