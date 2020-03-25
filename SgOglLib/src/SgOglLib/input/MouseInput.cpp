// This file is part of the SgOgl package.
// 
// Filename: MouseInput.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::input::MouseInput::MouseInput()
{
    ClearMouseStates();
}

sg::ogl::input::MouseInput::~MouseInput() noexcept
{
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

glm::ivec2 sg::ogl::input::MouseInput::GetCurrentPos() const
{
    return m_currentPosition;
}

glm::vec2 sg::ogl::input::MouseInput::GetDisplVec() const
{
    return m_displVec;
}

bool sg::ogl::input::MouseInput::IsLeftButtonPressed()
{
    return buttonStates.at(MouseButton::LEFT) == MouseState::PRESSED;
}

bool sg::ogl::input::MouseInput::IsRightButtonPressed()
{
    return buttonStates.at(MouseButton::RIGHT) == MouseState::PRESSED;
}

bool sg::ogl::input::MouseInput::IsLeftButtonReleased()
{
    return buttonStates.at(MouseButton::LEFT) == MouseState::RELEASED;
}

bool sg::ogl::input::MouseInput::IsRightButtonReleased()
{
    return buttonStates.at(MouseButton::RIGHT) == MouseState::RELEASED;
}

bool sg::ogl::input::MouseInput::IsScrolled() const
{
    return m_scrolled;
}

glm::vec2& sg::ogl::input::MouseInput::GetScrollOffset()
{
    return m_scrollOffset;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::input::MouseInput::SetPreviousPosition(const glm::ivec2& t_previousPosition)
{
    m_previousPosition = t_previousPosition;
}

void sg::ogl::input::MouseInput::SetCurrentPosition(const glm::ivec2& t_currentPosition)
{
    m_currentPosition = t_currentPosition;
}

void sg::ogl::input::MouseInput::SetDisplVec(const glm::vec2& t_displVec)
{
    m_displVec = t_displVec;
}

void sg::ogl::input::MouseInput::SetInWindow(const bool t_inWindow)
{
    m_inWindow = t_inWindow;
}

void sg::ogl::input::MouseInput::SetScrolled(const bool t_scrolled)
{
    m_scrolled = t_scrolled;
}

void sg::ogl::input::MouseInput::SetScrollOffset(const glm::vec2& t_scrollOffset)
{
    m_scrollOffset = t_scrollOffset;
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::input::MouseInput::Update()
{
    m_displVec.x = 0.0f;
    m_displVec.y = 0.0f;

    if (m_previousPosition.x > 0 && m_previousPosition.y > 0 && m_inWindow)
    {
        m_displVec.x = static_cast<float>(m_currentPosition.x - m_previousPosition.x);
        m_displVec.y = static_cast<float>(m_previousPosition.y - m_currentPosition.y);
    }

    m_previousPosition.x = m_currentPosition.x;
    m_previousPosition.y = m_currentPosition.y;
}

void sg::ogl::input::MouseInput::ClearMouseStates()
{
    for (auto& buttonState : buttonStates)
    {
        buttonState.second = MouseState::NONE;
    }
}
