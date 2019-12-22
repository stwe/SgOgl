// This file is part of the SgOgl package.
// 
// Filename: MouseInput.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

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

        [[nodiscard]] glm::ivec2 GetCurrentPos() const;
        [[nodiscard]] glm::vec2 GetDisplVec() const;
        [[nodiscard]] bool IsLeftButtonPressed() const;
        [[nodiscard]] bool IsRightButtonPressed() const;
        [[nodiscard]] bool IsScrolled() const;
        [[nodiscard]] glm::vec2& GetScrollOffset();

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPreviousPosition(const glm::ivec2& t_previousPosition);
        void SetCurrentPosition(const glm::ivec2& t_currentPosition);
        void SetDisplVec(const glm::vec2& t_displVec);
        void SetInWindow(bool t_inWindow);
        void SetLeftButtonPressed(bool t_leftButtonPressed);
        void SetRightButtonPressed(bool t_rightButtonPressed);
        void SetScrolled(bool t_scrolled);
        void SetScrollOffset(const glm::vec2& t_scrollOffset);

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

    protected:

    private:
        glm::ivec2 m_previousPosition{ glm::ivec2(-1, -1) };
        glm::ivec2 m_currentPosition{ glm::ivec2(0, 0) };
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };
        glm::vec2 m_scrollOffset{ glm::vec2(0.0f, 0.0f) };

        bool m_inWindow{ false };
        bool m_leftButtonPressed{ false };
        bool m_rightButtonPressed{ false };
        bool m_scrolled{ false };
    };
}
