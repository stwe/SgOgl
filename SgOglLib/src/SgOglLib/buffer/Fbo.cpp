#include "Fbo.h"
#include "Log.h"
#include "OpenGl.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Fbo::Fbo()
{
    GenerateFbo();
    BindFbo();

    SG_OGL_CORE_LOG_DEBUG("[Fbo::Fbo()] A new Fbo was created. Id: {}", m_fboId);
}

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
    // todo setze alte width && height
    //glViewport(0, 0, m_window.GetProjectionOptions().width, m_window.GetProjectionOptions().height);
}

void sg::ogl::buffer::Fbo::DeleteFbo() const
{
    if (m_fboId)
    {
        glDeleteFramebuffers(1, &m_fboId);
        SG_OGL_CORE_LOG_DEBUG("[Fbo::DeleteFbo()] Fbo was deleted. Id: {}", m_fboId);
    }
}

void sg::ogl::buffer::Fbo::BindAsRenderTarget() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboId);
    glViewport(0, 0, m_width, m_height);
}

//-------------------------------------------------
// Attachment
//-------------------------------------------------

void sg::ogl::buffer::Fbo::AttachTextures()
{
    // color buffer
    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

    // depth buffer
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // Check frame buffer.
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw SG_OGL_EXCEPTION("[Fbo::AttachTextures()] Error while creating Fbo Attachments.");
    }
}
