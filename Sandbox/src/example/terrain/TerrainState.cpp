// This file is part of the SgOgl package.
// 
// Filename: TerrainState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "TerrainState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool TerrainState::Input()
{
    m_scene->GetCurrentCamera().Input();

    m_terrainQuadtree->UpdateQuadtree();

    return true;
}

bool TerrainState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    return true;
}

void TerrainState::Render()
{
    m_terrainQuadtreeRenderSystem->Render();
    //m_guiRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TerrainState::Init()
{
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(0.5f, 750.0f, 0.5f),
        90.0f,
        -90.0f
    );
    m_firstPersonCamera->SetCameraVelocity(64.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    m_terrainQuadtree = std::make_shared<sg::ogl::terrain::TerrainQuadtree>(m_scene.get());
    GetApplicationContext()->GetEntityFactory().CreateTerrainQuadtreeEntity(m_terrainQuadtree);

    m_terrainQuadtreeRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>(m_scene.get());

    //m_guiRenderSystem = std::make_unique<sg::ogl::ecs::system::GuiRenderSystem>(m_scene.get());
    //GetApplicationContext()->GetEntityFactory().CreateGuiEntity(-0.65f, 0.65f, 0.25f, 0.25f, textureId);
}
