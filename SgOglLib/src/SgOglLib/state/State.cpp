#include "State.h"
#include "StateStack.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::state::State::Context::Context(Window& t_window, resource::ShaderManager& t_shaderManager)
    : window{ &t_window }
    , shaderManager{ &t_shaderManager }
{
}

sg::ogl::state::State::State(StateStack& t_stateStack, Context& t_context)
    : m_stateStack{ &t_stateStack }
    , m_context{  t_context }
{
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

sg::ogl::state::State::Context sg::ogl::state::State::GetContext() const
{
    return m_context;
}
