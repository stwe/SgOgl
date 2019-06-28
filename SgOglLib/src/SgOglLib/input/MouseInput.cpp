#include "MouseInput.h"
#include "Window.h"
#include "OpenGl.h"

sg::ogl::input::MouseInput& sg::ogl::input::MouseInput::GetInstance()
{
    static MouseInput instance;
    return instance;
}

void sg::ogl::input::MouseInput::Init(const Window& t_window)
{
    // Called when the cursor is moved.
    glfwSetCursorPosCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const double t_x, const double t_y)
        {
            GetInstance().m_currentPosition.x = static_cast<int>(t_x);
            GetInstance().m_currentPosition.y = static_cast<int>(t_y);
        }
    );

    // Called when the cursor enters or leaves the client area of the window.
    glfwSetCursorEnterCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const int t_entered)
        {
            GetInstance().m_inWindow = t_entered == 1;
        }
    );

    // Called when a mouse button is pressed or released.
    glfwSetMouseButtonCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const int t_button, const int t_action, const int t_mods)
        {
            GetInstance().m_leftButtonPressed = t_button == GLFW_MOUSE_BUTTON_1 && t_action == GLFW_PRESS;
            GetInstance().m_rightButtonPressed = t_button == GLFW_MOUSE_BUTTON_2 && t_action == GLFW_PRESS;
        }
    );
}

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

void sg::ogl::input::MouseInput::Input()
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
