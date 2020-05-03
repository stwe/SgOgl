// This file is part of the SgOgl package.
// 
// Filename: Water.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Water.h"
#include "Application.h"
#include "Core.h"
#include "buffer/WaterFbos.h"
#include "resource/TextureManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::water::Water::Water(
    Application* t_application,
    const float t_xPos,
    const float t_zPos,
    const float t_height,
    const glm::vec3& t_tileSize,
    const std::string& t_dudvMapFilePath,
    const std::string& t_normalMapFilePath
)
    : m_application{ t_application }
    , m_xPos{ t_xPos }
    , m_zPos{ t_zPos }
    , m_height{ t_height }
    , m_tileSize{ t_tileSize }
{
    SG_OGL_CORE_ASSERT(m_application, "[Water::Water()] Null pointer.");
    Log::SG_OGL_CORE_LOG_DEBUG("[Water::Water()] Create Water.");

    m_dudvTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_dudvMapFilePath);
    m_normalTextureId = m_application->GetTextureManager().GetTextureIdFromPath(t_normalMapFilePath);

    m_waterFbos = std::make_unique<buffer::WaterFbos>(m_application);
    SG_OGL_CORE_ASSERT(m_waterFbos, "[Water::Water()] Null pointer.");
}

sg::ogl::water::Water::~Water() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Water::~Water()] Destruct Water.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

float& sg::ogl::water::Water::GetXPosition()
{
    return m_xPos;
}

float& sg::ogl::water::Water::GetZPosition()
{
    return m_zPos;
}

float& sg::ogl::water::Water::GetHeight()
{
    return m_height;
}

glm::vec3& sg::ogl::water::Water::GetTileSize()
{
    return m_tileSize;
}

float sg::ogl::water::Water::GetWaveSpeed() const
{
    return m_waveSpeed;
}

float& sg::ogl::water::Water::GetWaveStrength()
{
    return m_waveStrength;
}

float& sg::ogl::water::Water::GetShineDamper()
{
    return m_shineDamper;
}

float& sg::ogl::water::Water::GetReflectivity()
{
    return m_reflectivity;
}

glm::vec3& sg::ogl::water::Water::GetWaterColor()
{
    return m_waterColor;
}

uint32_t sg::ogl::water::Water::GetDudvTextureId() const
{
    return m_dudvTextureId;
}

uint32_t sg::ogl::water::Water::GetNormalTextureId() const
{
    return m_normalTextureId;
}

const sg::ogl::buffer::WaterFbos& sg::ogl::water::Water::GetWaterFbos() const
{
    return *m_waterFbos;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::water::Water::SetWaveSpeed(const float t_waveSpeed)
{
    m_waveSpeed = t_waveSpeed;
}
