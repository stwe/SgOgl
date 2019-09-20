#include "TerrainComponent.h"
#include "Scene.h"
#include "terrain/Terrain.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::TerrainComponent::Init()
{
    // get ShaderManager
    auto& shaderManager{ m_parentEntity->GetParentScene()->GetApplicationContext()->GetShaderManager() };

    // get TerrainOptions
    const auto& terrainOptions{ m_terrain->GetTerrainOptions() };

    // get normalmap ShaderProgram
    auto& normalmapShaderProgram{ shaderManager->GetComputeShaderProgram(terrainOptions.normalmap.computeShaderName) };

    // get splatmap ShaderProgram
    auto& splatmapShaderProgram{ shaderManager->GetComputeShaderProgram(terrainOptions.splatmap.computeShaderName) };

    // run

    normalmapShaderProgram.Bind();
    normalmapShaderProgram.UpdateUniforms(*m_parentEntity);
    resource::ShaderProgram::Unbind();

    splatmapShaderProgram.Bind();
    splatmapShaderProgram.UpdateUniforms(*m_parentEntity);
    resource::ShaderProgram::Unbind();
}
