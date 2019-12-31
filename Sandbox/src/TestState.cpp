// This file is part of the SgOgl package.
// 
// Filename: TestState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "TestState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool TestState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool TestState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    m_playerRenderSystem->UpdateEntity(t_dt, m_player, m_currentAnimation, m_ticksPerSecond);

    const auto notInUse{ 0u };
    m_skeletalModelRenderSystem->UpdateEntity(t_dt, m_castleGuardIdle, notInUse, static_cast<float>(notInUse));

    return true;
}

void TestState::Render()
{
    m_playerRenderSystem->RenderEntity(m_player);
    m_skeletalModelRenderSystem->RenderEntity(m_castleGuardIdle);
    m_modelRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TestState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

#pragma region Cameras

    m_thirdPersonCamera = std::make_shared<sg::ogl::camera::ThirdPersonCamera>(
        GetApplicationContext(),
        m_playerPosition
    );

#pragma endregion Cameras

#pragma region Scene

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_thirdPersonCamera);

#pragma endregion Scene

#pragma region Lights

    // set ambient
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

#pragma endregion Lights

#pragma region RenderSystems

    m_playerRenderSystem = std::make_unique<sg::ogl::ecs::system::PlayerRenderSystem>(m_scene.get());
    m_skeletalModelRenderSystem = std::make_unique<sg::ogl::ecs::system::SkeletalModelRenderSystem>(m_scene.get());
    m_modelRenderSystem = std::make_unique<sg::ogl::ecs::system::ModelRenderSystem>(m_scene.get());

#pragma endregion RenderSystems

#pragma region Entities

    // a flat surface as "terrain" for our character
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Plane/plane1.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(32.0f, 1.0f, 32.0f),
        false,
        false,
        true, // the model has a normalmap
        false
    );

    // the animated character in the third person perspective - our player or game hero
    m_player = GetApplicationContext()->GetEntityFactory().CreateTppCharacterEntity(
        m_thirdPersonCamera,
        "res/model/Player/drone.X",
        m_playerPosition,
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        nullptr
    );

    // an other skeletal model
    m_castleGuardIdle = GetApplicationContext()->GetEntityFactory().CreateSkeletalModelEntity(
        "res/model/CastleGuard01/Idle.dae",
        glm::vec3(15.0f, 0.0f, 5.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0625f * 0.5f),
        false,
        false,
        true,
        false
    );

#pragma endregion Entities
}

void TestState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void TestState::RenderImGui()
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

void TestState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
