#include "State.h"
#include "StateStack.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::state::State::State(StateStack* const t_stateStack)
    : m_stateStack{ t_stateStack }
{
    SG_OGL_CORE_ASSERT(m_stateStack, "[State::State()] Null pointer.")
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
