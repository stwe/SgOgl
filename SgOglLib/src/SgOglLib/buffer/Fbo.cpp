#include "Fbo.h"
#include "Log.h"
#include "OpenGl.h"
#include "SgOglException.h"
#include "Application.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Fbo::Fbo(Application* t_application, const int32_t t_width, const int32_t t_height)
    : m_application{ t_application }
    , m_width{ t_width }
    , m_height{ t_height }
{
    SG_OGL_CORE_ASSERT(m_application, "[Fbo::Fbo()] Null pointer.")

    GenerateFbo();
    BindFbo();

    // no color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // create depth buffer attachment
    Attach();

    UnbindFbo();

    SG_OGL_CORE_LOG_DEBUG("[Fbo::Fbo()] A new Fbo was created. Id: {}", m_fboId);
}

sg::ogl::buffer::Fbo::~Fbo() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Fbo::~Fbo()] Destruct Fbo.");
    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::buffer::Fbo::GetDepthTextureId() const
{
    return m_depthTexture;
}

//-------------------------------------------------
// Fbo
//-------------------------------------------------

void sg::ogl::buffer::Fbo::GenerateFbo()
{
    glGenFramebuffers(1, &m_fboId);
}

void sg::ogl::buffer::Fbo::BindFbo() const
{
    SG_OGL_CORE_ASSERT(m_fboId, "[Fbo::BindFbo()] Invalid Fbo Id.")
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
}

void sg::ogl::buffer::Fbo::UnbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Fbo - Render Target
//-------------------------------------------------

void sg::ogl::buffer::Fbo::BindAsRenderTarget() const
{
    glViewport(0, 0, m_width, m_height);
    BindFbo();
}

void sg::ogl::buffer::Fbo::UnbindRenderTarget() const
{
    UnbindFbo();
    glViewport(0, 0, m_application->GetProjectionOptions().width, m_application->GetProjectionOptions().height);
}

//-------------------------------------------------
// Attachment
//-------------------------------------------------

void sg::ogl::buffer::Fbo::Attach()
{
    // depth buffer
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // check buffer
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw SG_OGL_EXCEPTION("[Fbo::Attach()] Error while creating Fbo Attachments.");
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::Fbo::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Fbo::CleanUp()] Start the OpenGL clean up process for Fbo. Id: {}", m_fboId);

    UnbindFbo();

    if (m_fboId)
    {
        glDeleteFramebuffers(1, &m_fboId);
        SG_OGL_CORE_LOG_DEBUG("[Fbo::DeleteFbo()] Fbo was deleted. Id: {}", m_fboId);
    }

    if (m_depthTexture)
    {
        glDeleteTextures(1, &m_depthTexture);
        SG_OGL_CORE_LOG_DEBUG("[Fbo::DeleteFbo()] Texture was deleted. Id: {}", m_depthTexture);
    }
}
