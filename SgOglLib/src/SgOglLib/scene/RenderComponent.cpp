#include "RenderComponent.h"
#include "resource/ShaderProgram.h"
#include "resource/Mesh.h"
#include "Log.h"
#include "Entity.h"
#include "OpenGl.h"

//-------------------------------------------------
// Render Config
//-------------------------------------------------

sg::ogl::scene::SkyboxRenderConfig::SkyboxRenderConfig(resource::ShaderProgram& t_shaderProgram)
    : RenderConfig(t_shaderProgram)
    , m_oldDepthFuncMode{ GL_LESS }
{}

void sg::ogl::scene::SkyboxRenderConfig::Enable()
{
    // save old depth func mode
    glGetIntegerv(GL_DEPTH_FUNC, &m_oldDepthFuncMode);

    // change depth func
    glDepthFunc(GL_LEQUAL);
}

void sg::ogl::scene::SkyboxRenderConfig::Disable()
{
    // restore old depth func mode
    glDepthFunc(m_oldDepthFuncMode);
}

const sg::ogl::scene::RenderConfig& sg::ogl::scene::RenderComponent::GetRenderConfig() const
{
    return *m_renderConfig;
}

void sg::ogl::scene::RenderComponent::SetRenderConfig(const RenderConfigSharedPtr& t_renderConfig)
{
    if (m_renderConfig)
    {
        m_renderConfig.reset();
    }

    m_renderConfig = t_renderConfig;
}

//-------------------------------------------------
// Render component
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

    resource::ShaderProgram::Unbind();
    m_renderConfig->Disable();
}
