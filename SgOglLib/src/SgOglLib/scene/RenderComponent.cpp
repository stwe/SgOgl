#include "RenderComponent.h"
#include "resource/ShaderProgram.h"
#include "Log.h"
#include "Entity.h"
#include "resource/Mesh.h"

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::scene::RenderConfig& sg::ogl::scene::RenderComponent::GetRenderConfig() const
{
    return *m_renderConfig;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::RenderComponent::SetRenderConfig(const RenderConfigSharedPtr& t_renderConfig)
{
    if (m_renderConfig)
    {
        m_renderConfig.reset();
    }

    m_renderConfig = t_renderConfig;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::RenderComponent::Render()
{
    SG_OGL_CORE_ASSERT(m_renderConfig, "[RenderComponent::Render()] Null pointer.")

    m_renderConfig->Enable();
    m_renderConfig->GetShaderProgram().Bind();

    m_renderConfig->GetShaderProgram().UpdateUniforms(*GetParentEntity());

    GetParentEntity()->mesh->InitDraw();
    GetParentEntity()->mesh->DrawPrimitives();
    GetParentEntity()->mesh->EndDraw();

    m_renderConfig->GetShaderProgram().Unbind();
    m_renderConfig->Disable();
}
