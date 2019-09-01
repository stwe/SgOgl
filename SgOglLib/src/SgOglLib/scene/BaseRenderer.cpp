#include "BaseRenderer.h"
#include "Scene.h"
#include "resource/ShaderManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::BaseRenderer::BaseRenderer(
    resource::ShaderManager& t_shaderManager,
    glm::mat4& t_projection
)
    : m_shaderManager{ t_shaderManager }
    , m_projectionMatrix{ t_projection }
{
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Scene* sg::ogl::scene::BaseRenderer::GetParentScene() const
{
    return m_parentScene;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::BaseRenderer::SetParentScene(Scene* t_scene)
{
    m_parentScene = t_scene;
}
