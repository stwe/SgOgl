#pragma once

#include <vector>
#include <map>
#include <functional>
#include "State.h"

namespace sg::ogl::state
{
    class StateStack
    {
    public:
        //-------------------------------------------------
        // Stack actions
        //-------------------------------------------------

        enum Action
        {
            PUSH,
            POP,
            CLEAR
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        StateStack() = delete;

        explicit StateStack(State::Context& t_context);

        StateStack(const StateStack& t_other) = delete;
        StateStack(StateStack&& t_other) noexcept = delete;
        StateStack& operator=(const StateStack& t_other) = delete;
        StateStack& operator=(StateStack&& t_other) noexcept = delete;

        ~StateStack() noexcept = default;

        //-------------------------------------------------
        // Handle States
        //-------------------------------------------------

        /**
         * @brief Register a factory function to create a new State on demand.
         * @tparam T The derived State class to register.
         * @param t_stateId 
         */
        template <typename T>
        void RegisterState(const StateId t_stateId)
        {
            m_factories[t_stateId] = [this]()
            {
                return std::make_unique<T>(*this, m_context);
            };
        }

        void Render();
        void Update(float t_dt);

        void PushState(StateId t_stateId);
        void PopState();
        void ClearStates();

        bool IsEmpty() const;

    protected:

    private:
        struct PendingChange
        {
            PendingChange() = delete;

            explicit PendingChange(const Action t_action, const StateId t_stateId = NONE)
                : action{ t_action }
                , stateId{ t_stateId }
            {}

            Action action;
            StateId stateId;
        };

        State::Context m_context;
        std::vector<State::StateUniquePtr> m_stack;
        std::vector<PendingChange> m_pendingList;
        std::map<StateId, std::function<State::StateUniquePtr()>> m_factories;

        /**
         * @brief Looking up the Id in the map and invoking the stored std::function factory.
         * @param t_stateId Id of a State.
         * @return Smart pointer to a newly created object of the corresponding State class.
         */
        State::StateUniquePtr CreateState(StateId t_stateId);

        void ApplyPendingChanges();
    };
}
