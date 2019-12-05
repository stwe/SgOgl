// This file is part of the SgOgl package.
// 
// Filename: EmptyState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "EmptyState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool EmptyState::Input()
{
    return true;
}

bool EmptyState::Update(double t_dt)
{
    return true;
}

void EmptyState::Render()
{
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void EmptyState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());
}
