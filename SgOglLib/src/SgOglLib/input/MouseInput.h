#pragma once

#include <glm/vec2.hpp>
#include "Core.h"

namespace sg::ogl
{
    class Window;
}

namespace sg::ogl::input
{
    class SG_OGL_API MouseInput
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MouseInput(const MouseInput& t_other) = delete;
        MouseInput(MouseInput&& t_other) noexcept = delete;
        MouseInput& operator=(const MouseInput& t_other) = delete;
        MouseInput& operator=(MouseInput&& t_other) noexcept = delete;

        //-------------------------------------------------
        // Singleton
        //-------------------------------------------------

        static MouseInput& GetInstance();

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        static void Init(const Window& t_window);

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        glm::ivec2 GetCurrentPos() const;
        glm::vec2 GetDisplVec() const;
        bool IsLeftButtonPressed() const;
        bool IsRightButtonPressed() const;

        //-------------------------------------------------
        // Input
        //-------------------------------------------------

        void Input();

    protected:

    private:
        MouseInput() = default;
        ~MouseInput() noexcept = default;

        glm::ivec2 m_previousPosition{ glm::ivec2(-1, -1) };
        glm::ivec2 m_currentPosition{ glm::ivec2(0, 0) };
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };

        bool m_inWindow{ false };
        bool m_leftButtonPressed{ false };
        bool m_rightButtonPressed{ false };
    };
}
