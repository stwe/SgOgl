// This file is part of the SgOgl package.
// 
// Filename: TerrainState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

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
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        m_scene->GetCurrentCamera().Update(t_dt);
        m_playerRenderSystem->UpdateEntity(t_dt, m_player, m_currentAnimation, m_ticksPerSecond);
    }

    return true;
}

void TerrainState::Render()
{
    m_terrainRenderSystem->Render();
    m_playerRenderSystem->RenderEntity(m_player);
    m_skyboxRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void TerrainState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create third person camera
    m_thirdPersonCamera = std::make_shared<sg::ogl::camera::ThirdPersonCamera>(
        GetApplicationContext(),
        m_playerPosition
    );

    // create scene
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());

    // scene init
    m_scene->SetCurrentCamera(m_thirdPersonCamera);
    m_scene->SetAmbientIntensity(glm::vec3(0.2f));

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(0.9f, -0.1f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.3f, 0.2f, 0.1f);
    m_sun->specularIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
    m_scene->SetDirectionalLight(m_sun);

    // create and add a point light to the scene
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(0.0f, 8.0f, 0.0f);
    m_pointLight->diffuseIntensity = glm::vec3(1.0f, 0.77f, 0.56f);
    m_pointLight->linear = 0.045f;
    m_pointLight->quadratic = 0.0075f;
    m_scene->AddPointLight(m_pointLight);

    // create render systems
    m_skyboxRenderSystem = std::make_unique<sg::ogl::ecs::system::SkyboxRenderSystem>(m_scene.get());
    m_playerRenderSystem = std::make_unique<sg::ogl::ecs::system::PlayerRenderSystem>(m_scene.get());
    m_skeletalModelRenderSystem = std::make_unique<sg::ogl::ecs::system::SkeletalModelRenderSystem>(m_scene.get());
    m_modelRenderSystem = std::make_unique<sg::ogl::ecs::system::ModelRenderSystem>(m_scene.get());
    m_terrainRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainRenderSystem>(m_scene.get());

    // create skybox entity
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };

    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);

    // create terrain
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(GetApplicationContext(), "res/config/Terrain.xml");
    m_terrain->GenerateTerrain<sg::ogl::resource::shaderprogram::ComputeNormalmap, sg::ogl::resource::shaderprogram::ComputeSplatmap>();

    GetApplicationContext()->GetEntityFactory().CreateTerrainEntity(m_terrain);

    // the animated character in the third person perspective - our player or game hero
    m_player = GetApplicationContext()->GetEntityFactory().CreateTppCharacterEntity(
        m_thirdPersonCamera,
        "res/model/Player/drone.X",
        m_playerPosition,
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        m_terrain
    );
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

    ImGui::Begin("Animation");

    if (ImGui::SmallButton("Anim 0"))
    {
        m_currentAnimation = 0;
    }

    if (ImGui::SmallButton("Anim 1"))
    {
        m_currentAnimation = 1;
    }

    if (ImGui::SmallButton("Anim 2"))
    {
        m_currentAnimation = 2;
    }

    if (ImGui::SmallButton("Anim 3"))
    {
        m_currentAnimation = 3;
    }

    if (ImGui::SmallButton("Anim 4"))
    {
        m_currentAnimation = 4;
    }

    ImGui::SliderFloat("Ticks per second", &m_ticksPerSecond, 1.0f, 2048.0f, "%.0f");

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
