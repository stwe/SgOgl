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
    return true;
}

bool TestState::Update(double t_dt)
{
    return true;
}

void TestState::Render()
{
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TestState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());
}
