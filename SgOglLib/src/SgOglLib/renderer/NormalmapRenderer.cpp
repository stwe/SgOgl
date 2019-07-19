#include "NormalmapRenderer.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "OpenGl.h"

uint32_t sg::ogl::renderer::NormalmapRenderer::ComputeNormalmap(
    resource::TextureManager& t_textureManager,
    resource::ShaderManager& t_shaderManager,
    const std::string& t_normalmapTextureName,
    const std::string& t_normalmapShaderName,
    const uint32_t t_heightmapTextureId,
    const int t_heightmapWidth,
    const float t_normalStrength
)
{
    // Get a new texture handle by name.
    const auto normalmapTextureId{ t_textureManager.GetTextureId(t_normalmapTextureName) };
    resource::TextureManager::Bind(normalmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    // Specify the amount of storage we want to use for the new normalmap texture.
    const int32_t numMipmaps{ 1 };
    glTexStorage2D(GL_TEXTURE_2D, numMipmaps, GL_RGBA32F, t_heightmapWidth, t_heightmapWidth);

    // Get the shader program by name.
    auto& computeShaderProgram{ t_shaderManager.GetComputeShaderProgram(t_normalmapShaderName) };
    computeShaderProgram->Bind();

    // Render to normalmap texture.
    glActiveTexture(GL_TEXTURE0);
    resource::TextureManager::BindForReading(t_heightmapTextureId, GL_TEXTURE0);
    computeShaderProgram->SetUniform("heightmap", 0);
    computeShaderProgram->SetUniform("heightmapWidth", t_heightmapWidth);
    computeShaderProgram->SetUniform("normalStrength", t_normalStrength);

    glBindImageTexture(0, normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(t_heightmapWidth / 16, t_heightmapWidth / 16, 1);

    resource::TextureManager::Bind(normalmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    resource::ShaderProgram::Unbind();

    return normalmapTextureId;
}
