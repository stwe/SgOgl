// This file is part of the SgOgl package.
// 
// Filename: Node.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtc/matrix_transform.hpp>
#include "Node.h"
#include "OpenGl.h"
#include "Application.h"
#include "Window.h"
#include "TerrainConfig.h"
#include "camera/Camera.h"
#include "resource/Mesh.h"
#include "resource/ModelManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "scene/Scene.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Node::Node(
    scene::Scene* t_scene,
    const TerrainConfigSharedPtr& t_terrainConfig,
    const int t_lod,
    const glm::vec2& t_location,
    const glm::vec2& t_index
)
    : m_scene{ t_scene }
    , m_terrainConfig{ t_terrainConfig }
    , m_lod{ t_lod }
    , m_location{ t_location }
    , m_index{ t_index }
{
    m_isLeaf = true;
    m_gap = 1.0f / (static_cast<float>(t_terrainConfig->rootNodes) * POW2_F[m_lod]);

    m_localTransform.scale = glm::vec3(m_gap, 0.0f, m_gap);
    m_localTransform.position = glm::vec3(m_location.x, 0.0f, m_location.y);

    m_worldTransform.scale = glm::vec3(t_terrainConfig->scaleXz, t_terrainConfig->scaleY, t_terrainConfig->scaleXz);
    m_worldTransform.position.x = -t_terrainConfig->scaleXz * 0.5f;
    m_worldTransform.position.z = -t_terrainConfig->scaleXz * 0.5f;
    m_worldTransform.position.y = 0.0f;

    ComputeCenterPosition();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::terrain::Node::Render(resource::ShaderProgram& t_shaderProgram, const MeshSharedPtr& t_patchMesh)
{
    if (m_isLeaf)
    {
        const auto projectionMatrix{ m_scene->GetApplicationContext()->GetWindow().GetProjectionMatrix() };

        t_shaderProgram.SetUniform("localMatrix", static_cast<glm::mat4>(m_localTransform));
        t_shaderProgram.SetUniform("worldMatrix", static_cast<glm::mat4>(m_worldTransform));
        t_shaderProgram.SetUniform("viewProjectionMatrix", projectionMatrix * m_scene->GetCurrentCamera().GetViewMatrix());

        t_shaderProgram.SetUniform("cameraPosition", m_scene->GetCurrentCamera().GetPosition());
        t_shaderProgram.SetUniform("scaleY", m_terrainConfig->scaleY);
        t_shaderProgram.SetUniform("lod", m_lod);
        t_shaderProgram.SetUniform("index", m_index);
        t_shaderProgram.SetUniform("gap", m_gap);
        t_shaderProgram.SetUniform("location", m_location);
        t_shaderProgram.SetUniform("lodMorphArea", m_terrainConfig->GetLodMorphingArea());

        t_shaderProgram.SetUniform("tessellationFactor", m_terrainConfig->tessellationFactor);
        t_shaderProgram.SetUniform("tessellationSlope", m_terrainConfig->tessellationSlope);
        t_shaderProgram.SetUniform("tessellationShift", m_terrainConfig->tessellationShift);

        t_shaderProgram.SetUniform("tessellationEnabled", m_terrainConfig->tessellationEnabled);
        t_shaderProgram.SetUniform("morphingEnabled", m_terrainConfig->morphingEnabled);

        t_shaderProgram.SetUniform("heightmap", 0);
        resource::TextureManager::BindForReading(m_terrainConfig->GetHeightmapTextureId(), GL_TEXTURE0);
        resource::TextureManager::UseBilinearFilter();

        t_shaderProgram.SetUniform("normalmap", 1);
        resource::TextureManager::BindForReading(m_terrainConfig->GetNormalmapTextureId(), GL_TEXTURE1);
        resource::TextureManager::UseBilinearFilter();

        t_shaderProgram.SetUniform("splatmap", 2);
        resource::TextureManager::BindForReading(m_terrainConfig->GetSplatmapTextureId(), GL_TEXTURE2);
        resource::TextureManager::UseBilinearFilter();

        t_shaderProgram.SetUniform("sand", 3);
        resource::TextureManager::BindForReading(m_terrainConfig->GetSandTextureId(), GL_TEXTURE3);

        t_shaderProgram.SetUniform("grass", 4);
        resource::TextureManager::BindForReading(m_terrainConfig->GetGrassTextureId(), GL_TEXTURE4);

        t_shaderProgram.SetUniform("rock", 5);
        resource::TextureManager::BindForReading(m_terrainConfig->GetRockTextureId(), GL_TEXTURE5);

        t_shaderProgram.SetUniform("snow", 6);
        resource::TextureManager::BindForReading(m_terrainConfig->GetSnowTextureId(), GL_TEXTURE6);

        t_patchMesh->InitDraw();
        t_patchMesh->DrawPrimitives(GL_PATCHES);
        t_patchMesh->EndDraw();
    }

    for (const auto& child : m_children)
    {
        child->Render(t_shaderProgram, t_patchMesh);
    }
}

void sg::ogl::terrain::Node::RenderWireframe(resource::ShaderProgram& t_shaderProgram, const MeshSharedPtr& t_patchMesh)
{
    if (m_isLeaf)
    {
        const auto projectionMatrix{ m_scene->GetApplicationContext()->GetWindow().GetProjectionMatrix() };

        t_shaderProgram.SetUniform("localMatrix", static_cast<glm::mat4>(m_localTransform));
        t_shaderProgram.SetUniform("worldMatrix", static_cast<glm::mat4>(m_worldTransform));
        t_shaderProgram.SetUniform("viewProjectionMatrix", projectionMatrix * m_scene->GetCurrentCamera().GetViewMatrix());

        t_shaderProgram.SetUniform("cameraPosition", m_scene->GetCurrentCamera().GetPosition());
        t_shaderProgram.SetUniform("scaleY", m_terrainConfig->scaleY);
        t_shaderProgram.SetUniform("lod", m_lod);
        t_shaderProgram.SetUniform("index", m_index);
        t_shaderProgram.SetUniform("gap", m_gap);
        t_shaderProgram.SetUniform("location", m_location);
        t_shaderProgram.SetUniform("lodMorphArea", m_terrainConfig->GetLodMorphingArea());

        t_shaderProgram.SetUniform("tessellationFactor", m_terrainConfig->tessellationFactor);
        t_shaderProgram.SetUniform("tessellationSlope", m_terrainConfig->tessellationSlope);
        t_shaderProgram.SetUniform("tessellationShift", m_terrainConfig->tessellationShift);

        t_shaderProgram.SetUniform("tessellationEnabled", m_terrainConfig->tessellationEnabled);
        t_shaderProgram.SetUniform("morphingEnabled", m_terrainConfig->morphingEnabled);

        t_shaderProgram.SetUniform("heightmap", 0);
        resource::TextureManager::BindForReading(m_terrainConfig->GetHeightmapTextureId(), GL_TEXTURE0);
        resource::TextureManager::UseBilinearFilter();

        t_patchMesh->InitDraw();
        t_patchMesh->DrawPrimitives(GL_PATCHES);
        t_patchMesh->EndDraw();
    }

    for (const auto& child : m_children)
    {
        child->RenderWireframe(t_shaderProgram, t_patchMesh);
    }
}

void sg::ogl::terrain::Node::Update()
{
    if (m_scene->GetCurrentCamera().GetPosition().y > m_terrainConfig->scaleY)
    {
        m_center.y = m_terrainConfig->scaleY;
    }
    else
    {
        m_center.y = m_scene->GetCurrentCamera().GetPosition().y;
    }

    const auto distance{ glm::distance(m_scene->GetCurrentCamera().GetPosition(), m_center) };
    const auto range{ m_terrainConfig->lodRanges[m_lod] };

    if (distance < range && m_lod < static_cast<int>(m_terrainConfig->lodRanges.size()) - 1)
    {
        Add4Children(m_lod + 1);
    }
    else if (distance >= range && !m_children.empty())
    {
        RemoveChildren();
    }

    for (const auto& child : m_children)
    {
        child->Update();
    }
}

//-------------------------------------------------
// Add / Remove
//-------------------------------------------------

void sg::ogl::terrain::Node::AddChild(NodeUniquePtr t_child)
{
    m_isLeaf = false;

    switch (m_lod)
    {
        case 0: t_child->m_color = glm::vec3(0.0f, 1.0f, 0.0f); break; // green
        case 1: t_child->m_color = glm::vec3(0.0f, 0.5f, 0.8f); break;
        case 2: t_child->m_color = glm::vec3(0.0f, 0.0f, 1.0f); break; // blue
        case 3: t_child->m_color = glm::vec3(0.0f, 0.5f, 0.5f); break;
        case 4: t_child->m_color = glm::vec3(1.0f, 1.0f, 0.0f); break; // yellow
        case 5: t_child->m_color = glm::vec3(0.8f, 0.2f, 0.0f); break;
        case 6: t_child->m_color = glm::vec3(0.9f, 0.1f, 0.0f); break;
        case 7: t_child->m_color = glm::vec3(1.0f, 0.0f, 0.0f); break; // red
        default:;
    }

    t_child->m_parent = this;
    m_children.push_back(std::move(t_child));
}

void sg::ogl::terrain::Node::Add4Children(const int t_lod)
{
    if (m_isLeaf)
    {
        m_isLeaf = false;
    }

    if (m_children.empty())
    {
        for (auto i{ 0 }; i < 2; ++i)
        {
            for (auto j{ 0 }; j < 2; ++j)
            {
                const auto loc{ m_location + glm::vec2(i * m_gap / 2.0f, j * m_gap / 2.0f) };
                AddChild(std::make_unique<Node>(m_scene, m_terrainConfig, t_lod, loc, glm::vec2(i, j)));
            }
        }
    }
}

void sg::ogl::terrain::Node::RemoveChildren()
{
    m_isLeaf = true;

    if (!m_children.empty())
    {
        m_children.clear();
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::terrain::Node::ComputeCenterPosition()
{
    auto loc{ m_location + m_gap * 0.5f };
    loc *= m_terrainConfig->scaleXz;
    loc -= m_terrainConfig->scaleXz * 0.5f;

    //const auto height{ GetTerrainHeight(loc.x, loc.y) };

    const auto height{ 0.0f };

    m_center = glm::vec3(loc.x, height, loc.y);
}
