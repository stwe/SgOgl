#include "TerrainRenderer.h"
#include "OpenGl.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "camera/LookAtCamera.h"
#include "terrain/Terrain.h"
#include "math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::renderer::TerrainRenderer::TerrainRenderer(
    resource::ShaderManager& t_shaderManager,
    resource::TextureManager& t_textureManager,
    camera::LookAtCamera& t_camera,
    glm::mat4& t_projection
)
    : m_shaderManager{ t_shaderManager }
    , m_textureManager{ t_textureManager }
    , m_camera{ t_camera }
    , m_projectionMatrix{ t_projection }
{
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::ogl::renderer::TerrainRenderer::Render(const terrain::Terrain& t_terrain, const std::string& t_shaderProgramName) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // get terrain options
    const auto& terrainOptions{ t_terrain.GetTerrainOptions() };

    // set transform
    math::Transform transform;
    transform.position = glm::vec3(terrainOptions.xPos, 0.0f, terrainOptions.zPos);
    const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * transform.GetModelMatrix() };
    shaderProgram->SetUniform("transform", mvp);

    // set textures
    int32_t counter{ 0 };
    for (const auto& entry : terrainOptions.texturePack)
    {
        shaderProgram->SetUniform(entry.first, counter);
        resource::TextureManager::BindForReading(m_textureManager.GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
        counter++;
    }

    // bind normalmap
    shaderProgram->SetUniform("normalmap", counter);
    resource::TextureManager::BindForReading(terrainOptions.normalmap.GetTextureId(), GL_TEXTURE0 + counter);
    counter++;

    // bind splatmap
    shaderProgram->SetUniform("splatmap", counter);
    resource::TextureManager::BindForReading(terrainOptions.splatmap.GetTextureId(), GL_TEXTURE0 + counter);

    // get terrain Mesh
    const auto& mesh{ t_terrain.GetMesh() };

    // render Mesh
    mesh->InitDraw();
    mesh->DrawPrimitives();
    mesh->EndDraw();

    shaderProgram->Unbind();
}

void sg::ogl::renderer::TerrainRenderer::RenderNormals(const terrain::Terrain& t_terrain, const std::string& t_shaderProgramName, const float t_normalLength) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // get terrain options
    const auto& terrainOptions{ t_terrain.GetTerrainOptions() };

    // set transform uniform
    math::Transform transform;
    transform.position = glm::vec3(terrainOptions.xPos, 0.0f, terrainOptions.zPos);
    const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * transform.GetModelMatrix() };
    shaderProgram->SetUniform("transform", mvp);

    // set normalLength uniform
    shaderProgram->SetUniform("normalLength", t_normalLength);

    // get terrain Mesh
    const auto& mesh{ t_terrain.GetMesh() };

    // render Mesh
    mesh->InitDraw();
    mesh->DrawPrimitives();
    mesh->EndDraw();

    shaderProgram->Unbind();
}
