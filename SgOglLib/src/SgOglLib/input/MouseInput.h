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
#include <unordered_map>

namespace sg::ogl
{
    class Window;
}

namespace sg::ogl::input
{
    class MouseInput
    {
    public:
        enum class MouseButton
        {
            LEFT,
            RIGHT
        };

        enum class MouseState
        {
            NONE,
            PRESSED,
            RELEASED
        };

        struct MouseButtonHash
        {
            std::size_t operator()(MouseButton t_mouseButton) const
            {
                return static_cast<std::size_t>(t_mouseButton);
            }
        };

        using MouseButtonStateContainer = std::unordered_map<MouseButton, MouseState>;

        inline static MouseButtonStateContainer buttonStates
        {
            { MouseButton::LEFT, MouseState::NONE },
            { MouseButton::RIGHT, MouseState::NONE }
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MouseInput();

        MouseInput(const MouseInput& t_other) = delete;
        MouseInput(MouseInput&& t_other) noexcept = delete;
        MouseInput& operator=(const MouseInput& t_other) = delete;
        MouseInput& operator=(MouseInput&& t_other) noexcept = delete;

        ~MouseInput() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] glm::ivec2 GetCurrentPos() const;
        [[nodiscard]] glm::vec2 GetDisplVec() const;

        [[nodiscard]] static bool IsLeftButtonPressed();
        [[nodiscard]] static bool IsRightButtonPressed();

        [[nodiscard]] static bool IsLeftButtonReleased();
        [[nodiscard]] static bool IsRightButtonReleased();

        [[nodiscard]] bool IsScrolled() const;
        [[nodiscard]] glm::vec2& GetScrollOffset();

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPreviousPosition(const glm::ivec2& t_previousPosition);
        void SetCurrentPosition(const glm::ivec2& t_currentPosition);
        void SetDisplVec(const glm::vec2& t_displVec);
        void SetInWindow(bool t_inWindow);
        void SetScrolled(bool t_scrolled);
        void SetScrollOffset(const glm::vec2& t_scrollOffset);

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

        static void ClearMouseStates();

    protected:

    private:
        glm::ivec2 m_previousPosition{ glm::ivec2(-1, -1) };
        glm::ivec2 m_currentPosition{ glm::ivec2(0, 0) };
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };
        glm::vec2 m_scrollOffset{ glm::vec2(0.0f, 0.0f) };

        bool m_inWindow{ false };
        bool m_scrolled{ false };
    };
}
