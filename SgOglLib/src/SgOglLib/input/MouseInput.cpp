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

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::input::MouseInput::Update()
{
    m_displVec.x = 0.0f;
    m_displVec.y = 0.0f;

    if (m_previousPosition.x > 0 && m_previousPosition.y > 0 && m_inWindow)
    {
        const auto deltax{ static_cast<float>(m_currentPosition.x - m_previousPosition.x) };
        const auto deltay{ static_cast<float>(m_currentPosition.y - m_previousPosition.y) };

        const auto rotateX = deltax != 0;
        const auto rotateY = deltay != 0;

        if (rotateX)
        {
            m_displVec.y = deltax;
            //std::cout << "deltaY: " << m_displVec.y << std::endl;
        }

        if (rotateY)
        {
            m_displVec.x = deltay;
            //std::cout << "deltaX: " << m_displVec.x << std::endl;
        }
    }

    m_previousPosition.x = m_currentPosition.x;
    m_previousPosition.y = m_currentPosition.y;
}
