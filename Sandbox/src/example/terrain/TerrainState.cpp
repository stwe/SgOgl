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

    return true;
}

bool TerrainState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    m_terrainQuadtreeRenderSystem->Update(t_dt);

    return true;
}

void TerrainState::Render()
{
    m_terrainQuadtreeRenderSystem->Render();
    //m_guiRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TerrainState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(0.0f, 400.0f, 0.0f),
        90.0f,
        -16.0f
    );
    m_firstPersonCamera->SetCameraVelocity(64.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    m_terrainConfig = std::make_shared<sg::ogl::terrain::TerrainConfig>(GetApplicationContext());
    m_terrainConfig->scaleXz = 1024.0f;
    m_terrainConfig->scaleY = 200.0f;
    m_terrainConfig->rootNodes = 2;
    m_terrainConfig->normalStrength = 10.0f;
    m_terrainConfig->lodRanges = { 1750, 874, 386, 192, 100, 50, 0, 0 };
    m_terrainConfig->InitMaps(
        "res/heightmap/heightmap_1024x1024x8.bmp",
        "normalmapTexture"
    );
    m_terrainConfig->InitMorphing();
    m_terrainConfig->InitTextures(
        "res/terrain/terrain2/sand.jpg",
        "res/terrain/terrain2/grass.jpg",
        "res/terrain/terrain2/rock2.jpg",
        "res/terrain/terrain2/snow.jpg"
    );

    m_terrainQuadtree = std::make_shared<sg::ogl::terrain::TerrainQuadtree>(m_scene.get(), m_terrainConfig);
    GetApplicationContext()->GetEntityFactory().CreateTerrainQuadtreeEntity(m_terrainQuadtree);

    m_terrainQuadtreeRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>(m_scene.get());

    //m_guiRenderSystem = std::make_unique<sg::ogl::ecs::system::GuiRenderSystem>(m_scene.get());
    //GetApplicationContext()->GetEntityFactory().CreateGuiEntity(-0.5f, 0.5f, 0.25f, 0.25f, m_terrainConfig->GetNormalmapTextureId());
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void TerrainState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    io.IniFilename = "res/config/Imgui.ini";

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void TerrainState::RenderImGui() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderFloat3("Camera", reinterpret_cast<float*>(&m_scene->GetCurrentCamera().GetPosition()), 0.0f, 2400.0f);

    ImGui::Separator();

    auto count{ 0 };
    m_terrainQuadtree->Count(count);
    ImGui::Text("Nodes count = %d", count);

    ImGui::Separator();

    ImGui::Checkbox("Morphing", &m_terrainConfig->morphingEnabled);
    ImGui::Checkbox("Tessellation", &m_terrainConfig->tessellationEnabled);

    ImGui::Separator();

    ImGui::SliderFloat("Scale Y", &m_terrainConfig->scaleY, 1.0f, 600.0f);

    ImGui::SliderInt("Tessellation Factor", &m_terrainConfig->tessellationFactor, 100, 1200);
    ImGui::SliderFloat("Tessellation Slope", &m_terrainConfig->tessellationSlope, 1.0f, 4.0f);
    ImGui::SliderFloat("Tessellation Shift", &m_terrainConfig->tessellationShift, 0.1f, 1.0f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TerrainState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
