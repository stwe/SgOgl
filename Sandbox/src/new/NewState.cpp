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

    return true;
}

void NewState::Render()
{
    m_scene->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void NewState::Init()
{
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext(), "res/scene/scene.lua");
}
