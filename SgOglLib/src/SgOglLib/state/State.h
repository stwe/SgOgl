#pragma once

#include "Core.h"

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

    class SG_OGL_API State
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        State() = delete;

        explicit State(StateStack* t_stateStack);

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept = default;

        //-------------------------------------------------
        // State logic
        //-------------------------------------------------

        virtual bool Input() = 0;
        virtual bool Update(float t_dt) = 0;
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
    };
}
