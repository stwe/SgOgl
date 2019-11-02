#pragma once

#include <glm/vec2.hpp>

namespace sg::ogl
{
    class Window;
}

namespace sg::ogl::input
{
    class MouseInput
    {
    public:
        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        glm::ivec2 GetCurrentPos() const;
        glm::vec2 GetDisplVec() const;
        bool IsLeftButtonPressed() const;
        bool IsRightButtonPressed() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPreviousPosition(const glm::ivec2& t_previousPosition);
        void SetCurrentPosition(const glm::ivec2& t_currentPosition);
        void SetDisplVec(const glm::vec2& t_displVec);
        void SetInWindow(bool t_inWindow);
        void SetLeftButtonPressed(bool t_leftButtonPressed);
        void SetRightButtonPressed(bool t_rightButtonPressed);

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

    protected:

    private:
        glm::ivec2 m_previousPosition{ glm::ivec2(-1, -1) };
        glm::ivec2 m_currentPosition{ glm::ivec2(0, 0) };
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };

        bool m_inWindow{ false };
        bool m_leftButtonPressed{ false };
        bool m_rightButtonPressed{ false };
    };
}
