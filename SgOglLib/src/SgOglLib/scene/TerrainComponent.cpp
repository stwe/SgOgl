#include "TerrainComponent.h"
#include "terrain/Terrain.h"
#include "Scene.h"
#include "OpenGl.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::TerrainComponent::Init()
{
    // get ShaderManager
    auto& shaderManager{ m_parentEntity->GetParentScene()->GetApplicationContext()->GetShaderManager() };

    // get TextureManager
    auto& textureManager{ m_parentEntity->GetParentScene()->GetApplicationContext()->GetTextureManager() };

    // get TerrainOptions
    const auto& terrainOptions{ m_terrain->GetTerrainOptions() };

    // get normalmap ShaderProgram
    auto& normalmapShaderProgram{ shaderManager.GetComputeShaderProgram(terrainOptions.normalmap.computeShaderName) };

    // get splatmap ShaderProgram
    auto& splatmapShaderProgram{ shaderManager.GetComputeShaderProgram(terrainOptions.splatmap.computeShaderName) };

    // render Normalmap

    const auto normalmapTextureId{ textureManager.GetTextureId(terrainOptions.normalmap.uniqueTextureName) };
    resource::TextureManager::Bind(normalmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    const auto numMipmaps{ log(m_terrain->GetHeightmapWidth()) / log(2) };
    glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, m_terrain->GetHeightmapWidth(), m_terrain->GetHeightmapWidth());

    normalmapShaderProgram.Bind();

    // todo
    //normalmapShaderProgram.UpdateUniforms(*m_parentEntity);

    glBindImageTexture(0, normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(m_terrain->GetHeightmapWidth() / 16, m_terrain->GetHeightmapWidth() / 16, 1);

    resource::ShaderProgram::Unbind();

    // render Splatmap

    const auto splatmapTextureId{ textureManager.GetTextureId(terrainOptions.splatmap.uniqueTextureName) };
    resource::TextureManager::Bind(splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, m_terrain->GetHeightmapWidth(), m_terrain->GetHeightmapWidth());

    splatmapShaderProgram.Bind();

    // todo
    //splatmapShaderProgram.UpdateUniforms(*m_parentEntity);

    glBindImageTexture(0, splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(m_terrain->GetHeightmapWidth() / 16, m_terrain->GetHeightmapWidth() / 16, 1);

    resource::ShaderProgram::Unbind();
}
