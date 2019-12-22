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

bool sg::ogl::input::MouseInput::IsLeftButtonPressed() const
{
    return m_leftButtonPressed;
}

bool sg::ogl::input::MouseInput::IsRightButtonPressed() const
{
    return m_rightButtonPressed;
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

void sg::ogl::input::MouseInput::SetLeftButtonPressed(const bool t_leftButtonPressed)
{
    m_leftButtonPressed = t_leftButtonPressed;
}

void sg::ogl::input::MouseInput::SetRightButtonPressed(const bool t_rightButtonPressed)
{
    m_rightButtonPressed = t_rightButtonPressed;
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

    // todo

    if (m_previousPosition.x > 0 && m_previousPosition.y > 0 && m_inWindow)
    {
        const auto deltax{ static_cast<float>(m_currentPosition.x - m_previousPosition.x) };
        const auto deltay{ static_cast<float>(m_previousPosition.y - m_currentPosition.y) };

        const auto rotateX = deltax != 0;
        const auto rotateY = deltay != 0;

        if (rotateX)
        {
            m_displVec.y = deltax;
        }

        if (rotateY)
        {
            m_displVec.x = deltay;
        }
    }

    m_previousPosition.x = m_currentPosition.x;
    m_previousPosition.y = m_currentPosition.y;
}
