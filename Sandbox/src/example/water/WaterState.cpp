// This file is part of the SgOgl package.
// 
// Filename: WaterState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "WaterState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool WaterState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool WaterState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    m_forwardRenderSystem->Update(t_dt);
    m_waterRenderSystem->Update(t_dt);

    return true;
}

void WaterState::Render()
{
    // render to textures
    m_waterRenderSystem->RenderReflectionTexture(m_forwardRenderSystem, m_skyboxRenderSystem, m_sunRenderSystem);
    m_waterRenderSystem->RenderRefractionTexture(m_forwardRenderSystem, m_skyboxRenderSystem, m_sunRenderSystem);

    // render to the screen
    m_forwardRenderSystem->Render();
    m_waterRenderSystem->Render();
    m_skyboxRenderSystem->Render();
    m_sunRenderSystem->Render();

    // render ImGui
    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void WaterState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(1338.0f, 739.0f, 831.0f),
        183.0f,
        -18.0f
    );
    m_firstPersonCamera->SetCameraVelocity(12.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };
    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Terrain/terrain.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(100.0f, 200.0f, 100.0f),
        false
    );

    // create water
    m_water = std::make_shared<sg::ogl::water::Water>(
        GetApplicationContext(),
        -220.0f, 500.0f,
        -66.0f,
        glm::vec3(1636.0f, 1.0f, 1318.0f),
        "res/texture/water/waterDUDV.png",
        "res/texture/water/normal.png"
    );
    GetApplicationContext()->GetEntityFactory().CreateWaterEntity(m_water);

    auto sunUniquePtr{ std::make_unique<sg::ogl::light::Sun>() };
    sunUniquePtr->direction = glm::vec3(0.55f, -0.34f, 0.0f);
    sunUniquePtr->diffuseIntensity = glm::vec3(1.0f, 0.8f, 0.5f);
    sunUniquePtr->specularIntensity = glm::vec3(0.5f);
    sunUniquePtr->textureId = GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/texture/sun/sun.png");
    sunUniquePtr->scale = 12.0f;

    GetApplicationContext()->GetEntityFactory().CreateSunEntity(std::move(sunUniquePtr));

    m_skyboxRenderSystem = std::make_unique<sg::ogl::ecs::system::SkyboxRenderSystem>(m_scene.get());
    m_forwardRenderSystem = std::make_unique<sg::ogl::ecs::system::ForwardRenderSystem>(m_scene.get());
    m_waterRenderSystem = std::make_unique<sg::ogl::ecs::system::WaterRenderSystem>(m_scene.get());
    m_sunRenderSystem = std::make_unique<sg::ogl::ecs::system::SunRenderSystem>(m_scene.get());
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void WaterState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    io.IniFilename = "res/config/Imgui.ini";

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void WaterState::RenderImGui() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderFloat("Water x position", &m_water->GetXPosition(), -2000.0f, 2000.0f);
    ImGui::SliderFloat("Water z position", &m_water->GetZPosition(), -2000.0f, 2000.0f);
    ImGui::SliderFloat3("Water tile size", reinterpret_cast<float*>(&m_water->GetTileSize()), -2000.0f, 2000.0f);
    ImGui::SliderFloat("Water height", &m_water->GetHeight(), -300.0f, 300.0f);

    ImGui::Separator();

    if (m_scene->HasDirectionalLight())
    {
        ImGui::SliderFloat3("Sun direction", reinterpret_cast<float*>(&m_scene->GetDirectionalLight().direction), -1.0f, 1.0f);
        ImGui::SliderFloat3("Sun specular", reinterpret_cast<float*>(&m_scene->GetDirectionalLight().specularIntensity), 0.0f, 1.0f);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WaterState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
