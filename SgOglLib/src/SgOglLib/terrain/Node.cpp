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
#include "camera/Camera.h"
#include "resource/Mesh.h"
#include "resource/ModelManager.h"
#include "resource/ShaderProgram.h"
#include "scene/Scene.h"
#include "TerrainQuadtree.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Node::Node(scene::Scene* t_scene, const std::string& t_name, const int t_lod, const glm::vec2& t_location, const glm::vec2& t_index)
    : name{ t_name }
    , lod{ t_lod }
    , location{ t_location }
    , index{ t_index }
    , m_scene{ t_scene }
{
    isLeaf = true;
    gap = 1.0f / (static_cast<float>(TerrainQuadtree::ROOT_NODES) * powf(2.0f, static_cast<float>(lod)));

    localTransform.scale = glm::vec3(gap, 0.0f, gap);
    localTransform.position = glm::vec3(location.x, 0.0f, location.y);

    worldTransform.scale = glm::vec3(TerrainQuadtree::SCALE_XZ, TerrainQuadtree::SCALE_Y, TerrainQuadtree::SCALE_XZ);
    worldTransform.position.x = -TerrainQuadtree::SCALE_XZ * 0.5f;
    worldTransform.position.z = -TerrainQuadtree::SCALE_XZ * 0.5f;
    worldTransform.position.y = 0.0f;

    ComputeCenterPosition();
}

//-------------------------------------------------
// Render
//-------------------------------------------------

void sg::ogl::terrain::Node::Render(resource::ShaderProgram& t_shaderProgram, const std::shared_ptr<resource::Mesh>& t_patchMesh)
{
    if (isLeaf)
    {
        const auto projectionMatrix{ m_scene->GetApplicationContext()->GetWindow().GetProjectionMatrix() };

        t_shaderProgram.SetUniform("localMatrix", static_cast<glm::mat4>(localTransform));
        t_shaderProgram.SetUniform("worldMatrix", static_cast<glm::mat4>(worldTransform));
        t_shaderProgram.SetUniform("viewProjectionMatrix", projectionMatrix * m_scene->GetCurrentCamera().GetViewMatrix());
        t_shaderProgram.SetUniform("color", color);

        t_patchMesh->InitDraw();
        t_patchMesh->DrawPrimitives(GL_PATCHES);
        t_patchMesh->EndDraw();
    }

    for (auto* child : children)
    {
        child->Render(t_shaderProgram, t_patchMesh);
    }
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::terrain::Node::Update()
{
    if (m_scene->GetCurrentCamera().GetPosition().y > TerrainQuadtree::SCALE_Y)
    {
        center.y = TerrainQuadtree::SCALE_Y;
    }
    else
    {
        center.y = m_scene->GetCurrentCamera().GetPosition().y;
    }

    const auto distance{ glm::distance(m_scene->GetCurrentCamera().GetPosition(), center) };
    const auto range{ lodRanges[lod] };

    if (distance < range && lod < 7)
    {
        Add4Children(lod + 1);
    }
    else if (distance >= range && !children.empty())
    {
        SG_OGL_CORE_LOG_WARN("Try to remove Nodes");
        RemoveChildren();
    }

    for (auto* child : children)
    {
        child->Update();
    }
}

//-------------------------------------------------
// Add / Remove
//-------------------------------------------------

void sg::ogl::terrain::Node::AddChild(Node* t_child)
{
    isLeaf = false;

    switch (lod)
    {
        case 0: t_child->color = glm::vec3(0.0f, 1.0f, 0.0f); break; // green
        case 1: t_child->color = glm::vec3(0.0f, 0.5f, 0.8f); break;
        case 2: t_child->color = glm::vec3(0.0f, 0.0f, 1.0f); break; // blue
        case 3: t_child->color = glm::vec3(0.0f, 0.5f, 0.5f); break;
        case 4: t_child->color = glm::vec3(1.0f, 1.0f, 0.0f); break; // yellow
        case 5: t_child->color = glm::vec3(0.8f, 0.2f, 0.0f); break;
        case 6: t_child->color = glm::vec3(0.9f, 0.1f, 0.0f); break;
        case 7: t_child->color = glm::vec3(1.0f, 0.0f, 0.0f); break; // red
        default:;
    }

    t_child->parent = this;
    children.push_back(t_child);
}

void sg::ogl::terrain::Node::Add4Children(const int t_lod)
{
    if (isLeaf)
    {
        isLeaf = false;
    }

    if (children.empty())
    {
        auto counter{ 0 };
        for (auto i{ 0 }; i < 2; ++i)
        {
            for (auto j{ 0 }; j < 2; ++j)
            {
                const auto loc{ location + glm::vec2(i * gap / 2.0f, j * gap / 2.0f) };
                const auto debugName{ std::string("child_" + std::to_string(counter++)) + std::string("_" + std::to_string(t_lod)) };
                auto* node{ new Node(m_scene, debugName, t_lod, loc, glm::vec2(i, j)) };
                AddChild(node);
            }
        }
    }
}

void sg::ogl::terrain::Node::RemoveChildren()
{
    isLeaf = true;

    if (!children.empty())
    {
        children.clear(); // todo
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::terrain::Node::ComputeCenterPosition()
{
    auto loc{ location + gap * 0.5f };
    loc *= TerrainQuadtree::SCALE_XZ;
    loc -= TerrainQuadtree::SCALE_XZ * 0.5f;

    //const auto height{ GetTerrainHeight(loc.x, loc.y) };

    const auto height{ 0.0f };

    center = glm::vec3(loc.x, height, loc.y);
}
