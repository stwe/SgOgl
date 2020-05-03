// This file is part of the SgOgl package.
// 
// Filename: WaterFbos.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "WaterFbos.h"
#include "OpenGl.h"
#include "SgOglException.h"
#include "Application.h"
#include "Core.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::WaterFbos::WaterFbos(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[WaterFbos::WaterFbos()] Null pointer.");

    m_reflectionWidth = m_application->GetProjectionOptions().width / 2;
    m_reflectionHeight = m_application->GetProjectionOptions().height / 2;

    m_refractionWidth = m_application->GetProjectionOptions().width;
    m_refractionHeight = m_application->GetProjectionOptions().height;

    InitReflectionFbo();
    InitRefractionFbo();

    Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::WaterFbos()] A new Reflection Fbo was created. Id: {}", m_reflectionFboId);
    Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::WaterFbos()] A new Refraction Fbo was created. Id: {}", m_refractionFboId);
}

sg::ogl::buffer::WaterFbos::~WaterFbos() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::~WaterFbos()] Destruct WaterFbos.");
    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::buffer::WaterFbos::GetReflectionFboId() const
{
    return m_reflectionFboId;
}

uint32_t sg::ogl::buffer::WaterFbos::GetReflectionColorTextureId() const
{
    return m_reflectionColorTextureId;
}

uint32_t sg::ogl::buffer::WaterFbos::GetRefractionFboId() const
{
    return m_refractionFboId;
}

uint32_t sg::ogl::buffer::WaterFbos::GetRefractionColorTextureId() const
{
    return m_refractionColorTextureId;
}

uint32_t sg::ogl::buffer::WaterFbos::GetRefractionDepthTextureId() const
{
    return m_refractionDepthTextureId;
}

//-------------------------------------------------
// Fbo
//-------------------------------------------------

uint32_t sg::ogl::buffer::WaterFbos::GenerateAndBindFbo()
{
    // generate Fbo
    uint32_t fboId;
    glGenFramebuffers(1, &fboId);

    // bind Fbo
    BindFbo(fboId);

    // always render to color attachment 0
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    return fboId;
}

void sg::ogl::buffer::WaterFbos::BindFbo(const uint32_t t_fboId)
{
    SG_OGL_CORE_ASSERT(t_fboId, "[WaterFbos::BindFbo()] Invalid Fbo Id.");
    glBindFramebuffer(GL_FRAMEBUFFER, t_fboId);
}

void sg::ogl::buffer::WaterFbos::UnbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Fbo - Render Target
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::BindAsRenderTarget(const uint32_t t_fbo, const int32_t t_width, const int32_t t_height)
{
    glViewport(0, 0, t_width, t_height);
    glBindTexture(GL_TEXTURE_2D, 0);
    BindFbo(t_fbo);
}

void sg::ogl::buffer::WaterFbos::BindReflectionFboAsRenderTarget() const
{
    BindAsRenderTarget(m_reflectionFboId, m_reflectionWidth, m_reflectionHeight);
}

void sg::ogl::buffer::WaterFbos::BindRefractionFboAsRenderTarget() const
{
    BindAsRenderTarget(m_refractionFboId, m_refractionWidth, m_refractionHeight);
}

void sg::ogl::buffer::WaterFbos::UnbindRenderTarget() const
{
    UnbindFbo();
    glViewport(0, 0, m_application->GetProjectionOptions().width, m_application->GetProjectionOptions().height);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::InitReflectionFbo()
{
    m_reflectionFboId = GenerateAndBindFbo();
    m_reflectionColorTextureId = CreateColorTextureAttachment(m_reflectionWidth, m_reflectionHeight);
    m_reflectionDepthRenderBufferId = CreateDepthRenderBufferAttachment(m_reflectionWidth, m_reflectionHeight);

    CheckFbo();

    UnbindFbo();
}

void sg::ogl::buffer::WaterFbos::InitRefractionFbo()
{
    m_refractionFboId = GenerateAndBindFbo();
    m_refractionColorTextureId = CreateColorTextureAttachment(m_refractionWidth, m_refractionHeight);
    m_refractionDepthTextureId = CreateDepthTextureAttachment(m_refractionWidth, m_refractionHeight);

    CheckFbo();

    UnbindFbo();
}

uint32_t sg::ogl::buffer::WaterFbos::CreateColorTextureAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    SG_OGL_CORE_ASSERT(textureId, "[WaterFbos::CreateColorTextureAttachment()] Invalid texture id.");

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

    return textureId;
}

uint32_t sg::ogl::buffer::WaterFbos::CreateDepthTextureAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    SG_OGL_CORE_ASSERT(textureId, "[WaterFbos::CreateDepthTextureAttachment()] Invalid texture id.");

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, t_width, t_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);

    return textureId;
}

uint32_t sg::ogl::buffer::WaterFbos::CreateDepthRenderBufferAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t depthRenderBufferId;
    glGenRenderbuffers(1, &depthRenderBufferId);

    SG_OGL_CORE_ASSERT(depthRenderBufferId, "[WaterFbos::CreateDepthRenderBufferAttachment()] Invalid depth render buffer id.");

    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t_width, t_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);

    return depthRenderBufferId;
}

void sg::ogl::buffer::WaterFbos::CheckFbo()
{
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        UnbindFbo();
        throw SG_OGL_EXCEPTION("[WaterFbos::CheckFbo()] Error while creating Fbo Attachments.");
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::CleanUp() const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Start the OpenGL clean up process for the WaterFbos objects.");

    if (m_reflectionFboId)
    {
        glDeleteFramebuffers(1, &m_reflectionFboId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Reflection Fbo was deleted. Id: {}", m_reflectionFboId);
    }

    if (m_reflectionColorTextureId)
    {
        glDeleteTextures(1, &m_reflectionColorTextureId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Reflection texture was deleted. Id: {}", m_reflectionColorTextureId);
    }

    if (m_reflectionDepthRenderBufferId)
    {
        glDeleteRenderbuffers(1, &m_reflectionDepthRenderBufferId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Depth render buffer was deleted. Id: {}", m_reflectionDepthRenderBufferId);
    }

    if (m_refractionFboId)
    {
        glDeleteFramebuffers(1, &m_refractionFboId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction Fbo was deleted. Id: {}", m_refractionFboId);
    }

    if (m_refractionColorTextureId)
    {
        glDeleteTextures(1, &m_refractionColorTextureId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction texture was deleted. Id: {}", m_refractionColorTextureId);
    }

    if (m_refractionDepthTextureId)
    {
        glDeleteTextures(1, &m_refractionDepthTextureId);
        Log::SG_OGL_CORE_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction depth texture was deleted. Id: {}", m_refractionDepthTextureId);
    }
}
