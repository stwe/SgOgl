#include "StateStack.h"
#include "SgOglException.h"

sg::ogl::state::StateStack::StateStack(State::Context t_context)
    : m_context{ t_context }
{
}

void sg::ogl::state::StateStack::Render()
{
    // Draw all active states from bottom to top.
    for (auto& state : m_stack)
    {
        state->Render();
    }
}

void sg::ogl::state::StateStack::Update(const float t_dt)
{
    // Iterate from top to bottom, stop as soon as Update() returns false.
    for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr)
    {
        if (!(*itr)->Update(t_dt))
        {
            break;
        }
    }

    ApplyPendingChanges();
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

sg::ogl::state::State::StateUniquePtr sg::ogl::state::StateStack::CreateState(const StateId t_stateId)
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
