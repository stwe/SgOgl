#include "SplatmapRenderer.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "OpenGl.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::renderer::SplatmapRenderer::SplatmapRenderer(const int t_heightmapWidth, std::string t_splatmapTextureName, resource::TextureManager& t_textureManager)
    : m_heightmapWidth{ t_heightmapWidth }
    , m_splatmapTextureName{ std::move(t_splatmapTextureName) }
{
    // Get a new texture handle.
    m_splatmapTextureId = t_textureManager.GetTextureId(m_splatmapTextureName);
    resource::TextureManager::Bind(m_splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    // Specify the amount of storage we want to use for the new splatmap texture.
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_heightmapWidth, m_heightmapWidth);
}

//-------------------------------------------------
// Compute splatmap
//-------------------------------------------------

// todo pass shader name
void sg::ogl::renderer::SplatmapRenderer::ComputeSplatmap(resource::ShaderManager& t_shaderManager, const uint32_t t_heightmapId, const float t_normalStrength) const
{
    auto& computeShaderProgram{ t_shaderManager.GetComputeShaderProgram("splatmap") };
    computeShaderProgram->Bind();

    glActiveTexture(GL_TEXTURE0);
    resource::TextureManager::BindForReading(t_heightmapId, GL_TEXTURE0);
    computeShaderProgram->SetUniform("heightmap", 0);
    computeShaderProgram->SetUniform("heightmapWidth", m_heightmapWidth);
    computeShaderProgram->SetUniform("normalStrength", t_normalStrength);

    glBindImageTexture(0, m_splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(m_heightmapWidth / 16, m_heightmapWidth / 16, 1);

    resource::TextureManager::Bind(m_splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    resource::ShaderProgram::Unbind();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::renderer::SplatmapRenderer::GetSplatmapTextureId() const
{
    return m_splatmapTextureId;
}
