// This file is part of the SgOgl package.
// 
// Filename: TerrainConfig.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "TerrainConfig.h"
#include "Application.h"
#include "OpenGl.h"
#include "Core.h"
#include "Log.h"
#include "resource/TextureManager.h"
#include "resource/shaderprogram/ComputeNormalmap.h"
#include "resource/shaderprogram/ComputeSplatmap.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::TerrainConfig::TerrainConfig(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[TerrainConfig::TerrainConfig()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[TerrainConfig::TerrainConfig()] Create TerrainConfig.");
}

sg::ogl::terrain::TerrainConfig::~TerrainConfig() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[TerrainConfig::~TerrainConfig()] Destruct TerrainConfig.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::terrain::TerrainConfig::GetHeightmapTextureId() const
{
    return m_heightmapTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetNormalmapTextureId() const
{
    return m_normalmapTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetSplatmapTextureId() const
{
    return m_splatmapTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetSandTextureId() const
{
    return m_sandTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetGrassTextureId() const
{
    return m_grassTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetRockTextureId() const
{
    return m_rockTextureId;
}

uint32_t sg::ogl::terrain::TerrainConfig::GetSnowTextureId() const
{
    return m_snowTextureId;
}

int sg::ogl::terrain::TerrainConfig::GetHeightmapWidth() const
{
    return m_heightmapWidth;
}

const sg::ogl::terrain::TerrainConfig::LodMorphingAreaContainer& sg::ogl::terrain::TerrainConfig::GetLodMorphingArea() const
{
    return m_lodMorphingArea;
}

sg::ogl::terrain::TerrainConfig::HeightmapHeightContainer& sg::ogl::terrain::TerrainConfig::GetHeightmapData()
{
    return m_heightmapData;
}

const sg::ogl::terrain::TerrainConfig::HeightmapHeightContainer& sg::ogl::terrain::TerrainConfig::GetHeightmapData() const
{
    return m_heightmapData;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::terrain::TerrainConfig::InitMapsAndMorphing(
    const std::string& t_heightmapFilePath,
    const std::string& t_normalmapTextureName,
    const std::string& t_splatmapTextureName
)
{
    LoadHeightmap(t_heightmapFilePath);
    LoadNormalmap(t_normalmapTextureName);
    LoadSplatmap(t_splatmapTextureName);

    InitMorphing();

    InitHeightmapData();
}

void sg::ogl::terrain::TerrainConfig::InitTextures(
    const std::string& t_sandFilePath,
    const std::string& t_grassFilePath,
    const std::string& t_rockFilePath,
    const std::string& t_snowFilePath
)
{
    m_sandTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_sandFilePath);
    m_grassTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_grassFilePath);
    m_rockTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_rockFilePath);
    m_snowTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_snowFilePath);
}

//-------------------------------------------------
// Load maps
//-------------------------------------------------

void sg::ogl::terrain::TerrainConfig::LoadHeightmap(const std::string& t_heightmapFilePath)
{
    m_heightmapTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_heightmapFilePath);
    m_heightmapWidth = m_application->GetTextureManager().GetMetadata(t_heightmapFilePath).width;

    SG_OGL_CORE_ASSERT(m_heightmapWidth, "[TerrainConfig::LoadHeightmap()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_heightmapWidth == m_application->GetTextureManager().GetMetadata(t_heightmapFilePath).height, "[TerrainConfig::LoadHeightmap()] Width and Height should have the same value.")
}

void sg::ogl::terrain::TerrainConfig::LoadNormalmap(const std::string& t_normalmapTextureName)
{
    m_normalmapTextureId = m_application->GetTextureManager().GetTextureId(t_normalmapTextureName);
    resource::TextureManager::Bind(m_normalmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    const auto numMipmaps{ log(m_heightmapWidth) / log(2) };
    glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, m_heightmapWidth, m_heightmapWidth);

    m_application->GetShaderManager().AddComputeShaderProgram<resource::shaderprogram::ComputeNormalmap>();
    auto& normalmapShaderProgram{ m_application->GetShaderManager().GetComputeShaderProgram<resource::shaderprogram::ComputeNormalmap>() };
    normalmapShaderProgram.Bind();
    normalmapShaderProgram.UpdateUniforms(*this);

    glBindImageTexture(0, m_normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(m_heightmapWidth / 16, m_heightmapWidth / 16, 1);

    resource::ShaderProgram::Unbind();
}

void sg::ogl::terrain::TerrainConfig::LoadSplatmap(const std::string& t_splatmapTextureName)
{
    m_splatmapTextureId = m_application->GetTextureManager().GetTextureId(t_splatmapTextureName);
    resource::TextureManager::Bind(m_splatmapTextureId);
    resource::TextureManager::UseBilinearFilter();

    const auto numMipmaps{ log(m_heightmapWidth) / log(2) };
    glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, m_heightmapWidth, m_heightmapWidth);

    m_application->GetShaderManager().AddComputeShaderProgram<resource::shaderprogram::ComputeSplatmap>();
    auto& splatmapShaderProgram{ m_application->GetShaderManager().GetComputeShaderProgram<resource::shaderprogram::ComputeSplatmap>() };
    splatmapShaderProgram.Bind();
    splatmapShaderProgram.UpdateUniforms(*this);

    glBindImageTexture(0, m_splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(m_heightmapWidth / 16, m_heightmapWidth / 16, 1);

    resource::ShaderProgram::Unbind();
}

void sg::ogl::terrain::TerrainConfig::InitMorphing()
{
    SG_OGL_CORE_ASSERT(!lodRanges.empty(), "[TerrainConfig::InitMorphing()] There are no values for the Lod Ranges.")
    SG_OGL_CORE_ASSERT(lodRanges.size() <= 10, "[TerrainConfig::InitMorphing()] There are to many values for the Lod Ranges.")

    for (auto i{ 0u }; i < lodRanges.size(); ++i)
    {
        if (lodRanges[i] == 0)
        {
            m_lodMorphingArea.push_back(0);
        }
        else
        {
            m_lodMorphingArea.push_back(
                lodRanges[i] - (static_cast<int>(scaleXz) / rootNodes) / POW2[i + 1]
            );
        }
    }
}

void sg::ogl::terrain::TerrainConfig::InitHeightmapData()
{
    resource::TextureManager::Bind(m_heightmapTextureId);

    // Create float buffer of red channel heightmap data.
    m_heightmapData.resize(m_heightmapWidth * m_heightmapWidth);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_heightmapData.data());
}
