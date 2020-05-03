// This file is part of the SgOgl package.
// 
// Filename: State.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "State.h"
#include "StateStack.h"
#include "Core.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::state::State::State(StateStack* const t_stateStack, std::string t_debugName)
    : m_stateStack{ t_stateStack }
    , m_debugName{ std::move(t_debugName) }
{
    SG_OGL_CORE_ASSERT(m_stateStack, "[State::State()] Null pointer.");

    Log::SG_OGL_CORE_LOG_DEBUG("[State::State()] Create State: " + m_debugName);
}

sg::ogl::state::State::~State() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[State::~State()] Destruct State: " + m_debugName);
}

//-------------------------------------------------
// Operations
//-------------------------------------------------

void sg::ogl::state::State::RequestStackPush(const StateId t_stateId) const
{
    m_stateStack->PushState(t_stateId);
}

void sg::ogl::state::State::RequestStackPop() const
{
    m_stateStack->PopState();
}

void sg::ogl::state::State::RequestStateClear() const
{
    m_stateStack->ClearStates();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::Application* sg::ogl::state::State::GetApplicationContext() const
{
    return m_stateStack->GetApplication();
}
