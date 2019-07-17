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

void sg::ogl::renderer::TerrainRenderer::Render(TerrainContainer& t_terrains, const std::string& t_shaderProgramName) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // for each terrain
    for (auto& terrain : t_terrains)
    {
        // set transform
        math::Transform transform;
        transform.position = glm::vec3(terrain->GetPosX(), 0.0f, terrain->GetPosZ());
        const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * transform.GetModelMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // get terrain Mesh
        auto& mesh{ terrain->GetMesh() };

        // set textures
        int32_t counter{ 0 };
        for (const auto& entry : terrain->GetTextureMap())
        {
            shaderProgram->SetUniform(entry.first, counter);
            resource::TextureManager::BindForReading(m_textureManager.GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
            counter++;
        }

        // bind splatmap
        shaderProgram->SetUniform("splatmap", counter);
        resource::TextureManager::BindForReading(m_textureManager.GetTextureId("splatmapTexture"), GL_TEXTURE0 + counter);

        // render Mesh
        mesh->InitDraw();
        mesh->DrawPrimitives();
        mesh->EndDraw();
    }

    shaderProgram->Unbind();
}

void sg::ogl::renderer::TerrainRenderer::RenderNormals(TerrainContainer& t_terrains, const std::string& t_shaderProgramName, const float t_normalLength) const
{
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(t_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // for each terrain
    for (auto& terrain : t_terrains)
    {
        // set transform uniform
        math::Transform transform;
        transform.position = glm::vec3(terrain->GetPosX(), 0.0f, terrain->GetPosZ());
        const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * transform.GetModelMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // set normalLength uniform
        shaderProgram->SetUniform("normalLength", t_normalLength);

        // get terrain Mesh
        auto& mesh{ terrain->GetMesh() };

        // render Mesh
        mesh->InitDraw();
        mesh->DrawPrimitives();
        mesh->EndDraw();
    }

    shaderProgram->Unbind();
}
