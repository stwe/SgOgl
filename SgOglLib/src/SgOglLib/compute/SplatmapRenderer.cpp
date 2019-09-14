#include "SplatmapRenderer.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "OpenGl.h"

uint32_t sg::ogl::compute::SplatmapRenderer::ComputeSplatmap(
    resource::TextureManager& t_textureManager,
    resource::ShaderManager& t_shaderManager,
    const std::string& t_splatmapTextureName,
    const std::string& t_splatmapShaderName,
    const uint32_t t_normalmapTextureId,
    const int t_heightmapWidth
)
{
    // Get a new texture handle by name.
    const auto splatmapTextureId{ t_textureManager.GetTextureId(t_splatmapTextureName) };
    resource::TextureManager::Bind(splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    // Specify the amount of storage we want to use for the new splatmap texture.
    const int32_t numMipmaps{ 1 };
    glTexStorage2D(GL_TEXTURE_2D, numMipmaps, GL_RGBA32F, t_heightmapWidth, t_heightmapWidth);

    // Get the shader program by name.
    auto& computeShaderProgram{ t_shaderManager.GetComputeShaderProgram(t_splatmapShaderName) };
    computeShaderProgram->Bind();

    // Render to splatmap texture.
    glActiveTexture(GL_TEXTURE0);
    resource::TextureManager::BindForReading(t_normalmapTextureId, GL_TEXTURE0);
    computeShaderProgram->SetUniform("normalmap", 0);
    computeShaderProgram->SetUniform("heightmapWidth", t_heightmapWidth);

    glBindImageTexture(0, splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(t_heightmapWidth / 16, t_heightmapWidth / 16, 1);

    resource::TextureManager::Bind(splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    resource::ShaderProgram::Unbind();

    return splatmapTextureId;
}
