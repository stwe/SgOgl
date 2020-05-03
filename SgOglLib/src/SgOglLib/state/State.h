// This file is part of the SgOgl package.
// 
// Filename: State.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::state
{
    /**
     * @brief All States have a unique Id.
     */
    enum StateId
    {
        NONE,
        TITLE,
        MENU,
        GAME,
        LOADING,
        PAUSE
    };

    class StateStack;

    class State
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        State() = delete;

        explicit State(StateStack* t_stateStack, std::string t_debugName = "State");

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept;

        //-------------------------------------------------
        // State logic
        //-------------------------------------------------

        virtual bool Input() = 0;
        virtual bool Update(double t_dt) = 0;
        virtual void Render() = 0;

    protected:
        //-------------------------------------------------
        // Operations
        //-------------------------------------------------

        void RequestStackPush(StateId t_stateId) const;
        void RequestStackPop() const;
        void RequestStateClear() const;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Application* GetApplicationContext() const;

    private:
        /**
         * @brief Pointer to the parent StateStack.
         */
        StateStack* m_stateStack{ nullptr };

        std::string m_debugName;
    };
}
