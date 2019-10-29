#include "RenderComponent.h"
#include "resource/ShaderProgram.h"
#include "resource/Mesh.h"
#include "scene/Entity.h"
#include "scene/Scene.h"
#include "scene/RenderConfig.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::component::RenderComponent::RenderComponent()
    : m_renderConfig{ std::make_shared<RenderConfig>() }
{
    SG_OGL_CORE_ASSERT(m_renderConfig, "[RenderComponent::RenderComponent()] Null pointer.")
}

//-------------------------------------------------
// Getter && Setter
//-------------------------------------------------

const sg::ogl::scene::RenderConfig& sg::ogl::scene::component::RenderComponent::GetRenderConfig() const
{
    SG_OGL_CORE_ASSERT(m_renderConfig, "[RenderComponent::GetRenderConfig()] Null pointer.")

    return *m_renderConfig;
}

void sg::ogl::scene::component::RenderComponent::SetRenderConfig(const RenderConfigSharedPtr& t_renderConfig)
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

void sg::ogl::scene::component::RenderComponent::Render()
{
    SG_OGL_CORE_ASSERT(m_renderConfig, "[RenderComponent::Render()] Null pointer.")

    // get shader program
    auto& shaderProgram{ m_parentEntity->GetParentScene()->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_renderConfig->shaderFolder) };

    // bind shader
    shaderProgram.Bind();

    // prepare render process
    m_renderConfig->PrepareRendering();

    // update uniforms
    // todo
    //shaderProgram.UpdateUniforms(*GetParentEntity());

    // bind Vao
    GetParentEntity()->mesh->InitDraw();

    // draw
    if (GetParentEntity()->instanceCount > 0)
    {
        GetParentEntity()->mesh->DrawInstanced(GetParentEntity()->instanceCount, m_renderConfig->drawMode);
    }
    else
    {
        GetParentEntity()->mesh->DrawPrimitives(m_renderConfig->drawMode);
    }

    // unbind Vao
    GetParentEntity()->mesh->EndDraw();

    // unbind shader program
    resource::ShaderProgram::Unbind();

    // finish render process
    m_renderConfig->FinishRendering();
}
