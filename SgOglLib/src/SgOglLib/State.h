#pragma once

#include "Core.h"

namespace sg::ogl
{
    class Application;

    class SG_OGL_API State
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        State() = default;

        State(const State& t_other) = delete;
        State(State&& t_other) noexcept = delete;
        State& operator=(const State& t_other) = delete;
        State& operator=(State&& t_other) noexcept = delete;

        virtual ~State() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        // todo

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetApplication(Application* t_application);

        //-------------------------------------------------
        // State logic
        //-------------------------------------------------

        virtual void Input() = 0;
        virtual void Update(float t_dt) = 0;
        virtual void Render(float t_dt) = 0;

    protected:

    private:
        Application* m_application{ nullptr };
    };
}
