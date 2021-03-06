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
    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Input();
    }

    // todo
    //sg::ogl::input::MouseInput::ClearMouseStates();

    return true;
}

bool NewState::Update(const double t_dt)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return true;
    }

    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Update(t_dt);
    }

    return true;
}

void NewState::Render()
{
    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Render();
    }

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void NewState::Init()
{
    InitImGui();

    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/start.lua");
    //m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/water.lua");
    //m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/sponza.lua");
    //m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/particles.lua");
    //m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/terrain.lua");
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

    GetApplicationContext()->registry.view<sg::ogl::light::Sun>().each([&](auto t_entity, auto& t_sunLight)
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
