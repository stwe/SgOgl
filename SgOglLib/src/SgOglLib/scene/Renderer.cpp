#include "Renderer.h"

// todo
#include "OpenGl.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "camera/LookAtCamera.h"
#include "math/Transform.h"
#include "Node.h"
#include "Scene.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Renderer::Renderer(
    resource::ShaderManager& t_shaderManager,
    resource::TextureManager& t_textureManager,
    glm::mat4& t_projection
)
    : m_shaderManager{ t_shaderManager }
    , m_textureManager{ t_textureManager }
    , m_projectionMatrix{ t_projection }
{
}

sg::ogl::scene::Renderer::~Renderer() noexcept
{
    // todo
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Scene* sg::ogl::scene::Renderer::GetParentScene() const
{
    return m_parentScene;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Renderer::SetParentScene(Scene* t_scene)
{
    m_parentScene = t_scene;
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

        // set transform
        // t_transform.GetModelMatrix()

        // todo
        const auto mvp{ m_projectionMatrix * m_parentScene->GetCamera().GetViewMatrix() * t_node.GetWorldMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // set ambient intensity
        shaderProgram->SetUniform("ambientIntensity", glm::vec3(1.0f));

        // get Material
        auto& material{ t_node.material };

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
