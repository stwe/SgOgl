// This file is part of the SgOgl package.
// 
// Filename: GBufferFbo.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "GBufferFbo.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "OpenGl.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::GBufferFbo::GBufferFbo(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[GBufferFbo::GBufferFbo()] Null pointer.")

    m_width = m_application->GetProjectionOptions().width;
    m_height = m_application->GetProjectionOptions().height;

    GenerateFbo();
    BindFbo();

    Attach();

    UnbindFbo();

    SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::GBufferFbo()] A new Fbo was created. Id: {}", m_fboId);
}

sg::ogl::buffer::GBufferFbo::~GBufferFbo() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::~GBufferFbo()] Destruct GBufferFbo.");
    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::buffer::GBufferFbo::GetFboId() const
{
    return m_fboId;
}

uint32_t sg::ogl::buffer::GBufferFbo::GetPositionTextureId() const
{
    return m_positionTextureId;
}

uint32_t sg::ogl::buffer::GBufferFbo::GetNormalTextureId() const
{
    return m_normalTextureId;
}

uint32_t sg::ogl::buffer::GBufferFbo::GetAlbedoSpecTextureId() const
{
    return m_albedoSpecTextureId;
}

//-------------------------------------------------
// Fbo
//-------------------------------------------------

void sg::ogl::buffer::GBufferFbo::GenerateFbo()
{
    glGenFramebuffers(1, &m_fboId);
}

void sg::ogl::buffer::GBufferFbo::BindFbo() const
{
    SG_OGL_CORE_ASSERT(m_fboId, "[GBufferFbo::GBufferFbo()] Invalid Fbo Id.")
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
}

void sg::ogl::buffer::GBufferFbo::UnbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void sg::ogl::buffer::GBufferFbo::CopyDepthBufferToDefaultFramebuffer() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Attachment
//-------------------------------------------------

void sg::ogl::buffer::GBufferFbo::Attach()
{
    // position color buffer
    glGenTextures(1, &m_positionTextureId);
    SG_OGL_CORE_ASSERT(m_positionTextureId, "[GBufferFbo::Attach()] Invalid texture Id.")
    glBindTexture(GL_TEXTURE_2D, m_positionTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionTextureId, 0);

    // normal color buffer
    glGenTextures(1, &m_normalTextureId);
    SG_OGL_CORE_ASSERT(m_normalTextureId, "[GBufferFbo::Attach()] Invalid texture Id.")
    glBindTexture(GL_TEXTURE_2D, m_normalTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTextureId, 0);

    // color + specular color buffer
    glGenTextures(1, &m_albedoSpecTextureId);
    SG_OGL_CORE_ASSERT(m_albedoSpecTextureId, "[GBufferFbo::Attach()] Invalid texture Id.")
    glBindTexture(GL_TEXTURE_2D, m_albedoSpecTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_albedoSpecTextureId, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    const std::vector<uint32_t> gbufferAttachments{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(static_cast<int32_t>(gbufferAttachments.size()), gbufferAttachments.data());

    // create and attach depth buffer (renderbuffer)
    uint32_t rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // check buffer
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw SG_OGL_EXCEPTION("[GBufferFbo::Attach()] Error while creating Fbo Attachments.");
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::GBufferFbo::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::CleanUp()] Start the OpenGL clean up process for Fbo. Id: {}", m_fboId);

    UnbindFbo();

    if (m_fboId)
    {
        glDeleteFramebuffers(1, &m_fboId);
        SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::CleanUp()] Fbo was deleted. Id: {}", m_fboId);
    }

    if (m_positionTextureId)
    {
        glDeleteTextures(1, &m_positionTextureId);
        SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::CleanUp()] Position texture was deleted. Id: {}", m_positionTextureId);
    }

    if (m_normalTextureId)
    {
        glDeleteTextures(1, &m_normalTextureId);
        SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::CleanUp()] Normal texture was deleted. Id: {}", m_normalTextureId);
    }

    if (m_albedoSpecTextureId)
    {
        glDeleteTextures(1, &m_albedoSpecTextureId);
        SG_OGL_CORE_LOG_DEBUG("[GBufferFbo::CleanUp()] Albedo texture was deleted. Id: {}", m_albedoSpecTextureId);
    }
}
