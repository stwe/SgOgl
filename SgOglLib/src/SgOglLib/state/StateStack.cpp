#include "StateStack.h"
#include "SgOglException.h"
#include "Log.h"
#include "Core.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::state::DeleteState::operator()(State* t_state) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteState::operator()] Delete State.");
    delete t_state;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::state::StateStack::StateStack(Application* const t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[StateStack::StateStack()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[StateStack::StateStack()] Create StateStack.");
}

sg::ogl::state::StateStack::~StateStack() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[StateStack::~StateStack()] Destruct StateStack.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::Application* sg::ogl::state::StateStack::GetApplication() const
{
    return m_application;
}

//-------------------------------------------------
// Handle States
//-------------------------------------------------

void sg::ogl::state::StateStack::Input()
{
    // Iterate from top to bottom, stop as soon as Input() returns false.
    for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr)
    {
        if (!(*itr)->Input())
        {
            break;
        }
    }

    ApplyPendingChanges();
}

void sg::ogl::state::StateStack::Update(const double t_dt)
{
    // Iterate from top to bottom, stop as soon as Update() returns false.
    for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr)
    {
        if (!(*itr)->Update(t_dt))
        {
            break;
        }
    }

    ApplyPendingChanges();
}

void sg::ogl::state::StateStack::Render()
{
    // Draw all active states from bottom to top.
    for (auto& state : m_stack)
    {
        state->Render();
    }
}

void sg::ogl::state::StateStack::PushState(const StateId t_stateId)
{
    m_pendingList.emplace_back(PUSH, t_stateId);
}

void sg::ogl::state::StateStack::PopState()
{
    m_pendingList.emplace_back(POP);
}

void sg::ogl::state::StateStack::ClearStates()
{
    m_pendingList.emplace_back(CLEAR);
}

bool sg::ogl::state::StateStack::IsEmpty() const
{
    return m_stack.empty();
}

sg::ogl::state::StateStack::StateUniquePtr sg::ogl::state::StateStack::CreateState(const StateId t_stateId)
{
    if (m_factories.count(t_stateId) == 0)
    {
        throw SG_OGL_EXCEPTION("[StateStack::CreateState()] There is no factory function for the given StateId.");
    }

    return m_factories.at(t_stateId)();
}

void sg::ogl::state::StateStack::ApplyPendingChanges()
{
    for (auto& change : m_pendingList)
    {
        switch (change.action)
        {
            case PUSH:
                m_stack.push_back(CreateState(change.stateId));
                break;

            case POP:
                m_stack.pop_back();
                break;

            case CLEAR:
                m_stack.clear();
                break;
        }
    }

    m_pendingList.clear();
}
