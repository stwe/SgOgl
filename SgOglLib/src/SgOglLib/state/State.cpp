#include "State.h"
#include "StateStack.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::state::State::Context::Context(Window& t_window, resource::ShaderManager& t_shaderManager, resource::TextureManager& t_textureManager)
    : window{ &t_window }
    , shaderManager{ &t_shaderManager }
    , textureManager{ &t_textureManager }
{
    SG_OGL_CORE_ASSERT(window, "[State::Context::Context()] Null pointer.")
    SG_OGL_CORE_ASSERT(shaderManager, "[State::Context::Context()] Null pointer.")
    SG_OGL_CORE_ASSERT(textureManager, "[State::Context::Context()] Null pointer.")
}

sg::ogl::state::State::State(StateStack& t_stateStack, Context& t_context)
    : m_stateStack{ &t_stateStack }
    , m_context{  t_context }
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

// todo
sg::ogl::state::State::Context sg::ogl::state::State::GetContext() const
{
    return m_context;
}
