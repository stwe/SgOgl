// This file is part of the SgOgl package.
// 
// Filename: ForwardRenderingState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "ForwardRenderingState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool ForwardRenderingState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool ForwardRenderingState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);
    m_forwardRenderSystem->Update(t_dt);

    m_temp += static_cast<float>(t_dt);

    auto& pointLights{ m_scene->GetScenePointLights() };
    for (auto& pointLight : pointLights)
    {
        pointLight->position.x += sinf(m_temp);
    }

    return true;
}

void ForwardRenderingState::Render()
{
    m_forwardRenderSystem->Render();
    m_skyboxRenderSystem->Render();
    m_sunRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void ForwardRenderingState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(308.0f, 176.0f, 268.0f),
        -131.0f,
        -6.0f
    );
    m_firstPersonCamera->SetCameraVelocity(24.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    AddScenePointLights(4);
    AddEntityPointLights();
    AddEntityDirectionalLight("res/texture/sun/sun.png");

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Plane/plane1.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(100.0f, 1.0f, 100.0f),
        false
    );

    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };
    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);

    m_skyboxRenderSystem = std::make_unique<sg::ogl::ecs::system::SkyboxRenderSystem>(m_scene.get());
    m_forwardRenderSystem = std::make_unique<sg::ogl::ecs::system::ForwardRenderSystem>(m_scene.get());
    m_sunRenderSystem = std::make_unique<sg::ogl::ecs::system::SunRenderSystem>(m_scene.get());
}

void ForwardRenderingState::AddScenePointLights(const int t_numPointLights) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> pos(-50.0f, 50.0f);
    const std::uniform_real_distribution<float> col(0.0f, 3.0f);

    for (auto i{ 0 }; i < t_numPointLights; ++i)
    {
        auto pointLight{ std::make_unique<sg::ogl::light::PointLight>() };
        pointLight->position = glm::vec3(pos(engine), 4.0f, pos(engine));
        pointLight->diffuseIntensity = glm::vec3(col(engine), col(engine), col(engine));
        pointLight->linear = 0.045f;
        pointLight->quadratic = 0.0075f;
        m_scene->AddScenePointLight(std::move(pointLight));
    }
}

void ForwardRenderingState::AddEntityPointLights() const
{
    auto pointLightUniquePtr0{ std::make_unique<sg::ogl::light::PointLight>() };
    pointLightUniquePtr0->position = glm::vec3(0.0f, 5.0f, 0.0f);
    pointLightUniquePtr0->diffuseIntensity = glm::vec3(10.0f, 0.0f, 0.0f);
    pointLightUniquePtr0->linear = 0.045f;
    pointLightUniquePtr0->quadratic = 0.0075f;

    GetApplicationContext()->GetEntityFactory().CreatePointLightEntity(
        std::move(pointLightUniquePtr0),
        "PointLight0",
        "res/model/Lamp/Lamp.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-90.0f, 0.0f, 0.0f),
        glm::vec3(0.25f),
        false
    );

    auto pointLightUniquePtr1{ std::make_unique<sg::ogl::light::PointLight>() };
    pointLightUniquePtr1->position = glm::vec3(-55.0f, 5.0f, 0.0f);
    pointLightUniquePtr1->diffuseIntensity = glm::vec3(0.0f, 0.0f, 10.0f);
    pointLightUniquePtr1->linear = 0.045f;
    pointLightUniquePtr1->quadratic = 0.0075f;

    GetApplicationContext()->GetEntityFactory().CreatePointLightEntity(
        std::move(pointLightUniquePtr1),
        "PointLight1",
        "res/model/Lamp/Lamp.obj",
        glm::vec3(-55.0f, 0.0f, 0.0f),
        glm::vec3(-90.0f, 0.0f, 0.0f),
        glm::vec3(0.25f),
        false
    );
}

void ForwardRenderingState::AddEntityDirectionalLight(const std::string& t_sunTexturePath) const
{
    auto sunUniquePtr{ std::make_unique<sg::ogl::light::Sun>() };
    sunUniquePtr->direction = glm::vec3(0.55f, -0.34f, 1.0f);
    sunUniquePtr->diffuseIntensity = glm::vec3(1.0f, 0.8f, 0.5f);
    sunUniquePtr->specularIntensity = glm::vec3(1.0f, 1.0f, 1.0f);
    sunUniquePtr->textureId = GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(t_sunTexturePath);
    sunUniquePtr->scale = 12.0f;

    GetApplicationContext()->GetEntityFactory().CreateSunEntity(std::move(sunUniquePtr));
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void ForwardRenderingState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    io.IniFilename = "res/config/Imgui.ini";

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void ForwardRenderingState::RenderImGui() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Sun");

    if (m_scene->HasDirectionalLight())
    {
        ImGui::SliderFloat3("Sun direction", reinterpret_cast<float*>(&m_scene->GetDirectionalLight().direction), -1.0f, 1.0f);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ForwardRenderingState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
