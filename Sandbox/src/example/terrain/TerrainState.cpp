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
// Ctors. / Dtor.
//-------------------------------------------------

TerrainState::TerrainState(sg::ogl::state::StateStack* t_stateStack)
    : State{ t_stateStack, "TerrainState" }
{
    Init();
}

TerrainState::~TerrainState() noexcept
{
    SG_OGL_LOG_DEBUG("[TerrainState::~TerrainState()] Destruct TerrainState.");
    CleanUpImGui();
}

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

    if (m_renderWireframe)
    {
        m_terrainQuadtreeWfRenderSystem->Update(t_dt);
    }
    else
    {
        m_terrainQuadtreeRenderSystem->Update(t_dt);
    }

    return true;
}

void TerrainState::Render()
{
    if (m_renderWireframe)
    {
        m_terrainQuadtreeWfRenderSystem->Render();
    }
    else
    {
        m_terrainQuadtreeRenderSystem->Render();
    }

    m_skydomeRenderSystem->Render();
    m_sunRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TerrainState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(2542.0f, 1754.0f, 2357.0f),
        -125.0f,
        -21.0f
    );
    m_firstPersonCamera->SetCameraVelocity(256.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    m_terrainConfig = std::make_shared<sg::ogl::terrain::TerrainConfig>(GetApplicationContext());
    m_terrainConfig->scaleXz = 8000.0f;
    m_terrainConfig->scaleY = 1700.0f;
    m_terrainConfig->rootNodes = 12;
    m_terrainConfig->normalStrength = 60.0f;
    m_terrainConfig->lodRanges = { 1750, 874, 386, 192, 100, 50, 0, 0 };
    m_terrainConfig->use16BitHeightmap = true;
    m_terrainConfig->InitMapsAndMorphing(
        "res/heightmap/Ruhpolding8km.png",
        "normalmapTexture", // todo auto generate names heightmap path + normalmap etc.
        "splatmapTexture"
    );
    m_terrainConfig->InitTextures(
        "res/terrain/terrain0/Grass (Hill).jpg",
        "res/terrain/terrain1/Grass.jpg",
        "res/terrain/terrain1/moss.jpg",
        "res/terrain/terrain0/Cliff (Layered Rock).jpg"
    );

    m_terrainQuadtree = std::make_shared<sg::ogl::terrain::TerrainQuadtree>(m_scene.get(), m_terrainConfig);
    GetApplicationContext()->GetEntityFactory().CreateTerrainQuadtreeEntity(m_terrainQuadtree);

    m_terrainQuadtreeRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>(m_scene.get());
    m_terrainQuadtreeWfRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeWfRenderSystem>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<sg::ogl::ecs::system::SkydomeRenderSystem>(m_scene.get());
    m_sunRenderSystem = std::make_unique<sg::ogl::ecs::system::SunRenderSystem>(m_scene.get());

    GetApplicationContext()->GetEntityFactory().CreateSkydomeEntity("res/model/Dome/dome.obj");

    m_sun = std::make_shared<sg::ogl::light::Sun>();
    m_sun->direction = glm::vec3(0.55f, -1.0f, -1.0f);
    m_sun->diffuseIntensity = glm::vec3(1.2f, 1.2f, 1.2f);
    m_sun->textureId = GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/sun/sun.png");
    m_sun->scale = 12.0f;

    GetApplicationContext()->GetEntityFactory().CreateSunEntity(m_sun);

    m_scene->SetCurrentDirectionalLight(m_sun);
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

void TerrainState::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderFloat3("Camera", reinterpret_cast<float*>(&m_scene->GetCurrentCamera().GetPosition()), 0.0f, 6300.0f);

    ImGui::Separator();

    auto count{ 0 };
    m_terrainQuadtree->Count(count);
    ImGui::Text("Nodes count = %d", count);

    ImGui::Separator();

    ImGui::Checkbox("Morphing", &m_terrainConfig->morphingEnabled);
    ImGui::Checkbox("Tessellation", &m_terrainConfig->tessellationEnabled);
    ImGui::Checkbox("Wireframe", &m_renderWireframe);

    ImGui::Separator();

    ImGui::SliderFloat("Scale Y", &m_terrainConfig->scaleY, 1.0f, 2000.0f);

    ImGui::SliderInt("Tessellation Factor", &m_terrainConfig->tessellationFactor, 100, 1200);
    ImGui::SliderFloat("Tessellation Slope", &m_terrainConfig->tessellationSlope, 1.0f, 4.0f);
    ImGui::SliderFloat("Tessellation Shift", &m_terrainConfig->tessellationShift, -1.0f, 1.0f);

    ImGui::Separator();

    ImGui::SliderFloat3("Sun direction", reinterpret_cast<float*>(&m_scene->GetCurrentDirectionalLight().direction), -1.0f, 1.0f);

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
