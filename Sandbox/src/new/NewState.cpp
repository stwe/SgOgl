// This file is part of the SgOgl package.
// 
// Filename: NewState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "NewState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool NewState::Input()
{
    m_scene->Input();

    return true;
}

bool NewState::Update(const double t_dt)
{
    m_scene->Update(t_dt);

    m_temp += static_cast<float>(t_dt);

    m_scene->GetApplicationContext()->registry.view<sg::ogl::light::PointLight>(
        entt::exclude<sg::ogl::ecs::component::ModelComponent>
        ).each([&](auto t_entity, auto& t_pointLight)
    {
        t_pointLight.position.x += sinf(m_temp);
    });

    return true;
}

void NewState::Render()
{
    m_scene->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void NewState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext(), "res/scene/rendererTest.lua");
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void NewState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    io.IniFilename = "res/config/Imgui.ini";

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void NewState::RenderImGui() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    m_scene->GetApplicationContext()->registry.view<sg::ogl::light::Sun>().each([&](auto t_entity, auto& t_sunLight)
    {
        ImGui::SliderFloat3("Sun direction", reinterpret_cast<float*>(&t_sunLight.direction), -1.0f, 1.0f);
    });

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void NewState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
