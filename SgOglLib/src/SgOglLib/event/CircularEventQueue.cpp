// This file is part of the SgOgl package.
// 
// Filename: CircularEventQueue.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "CircularEventQueue.h"
#include "OpenGl.h"
#include "Core.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::event::CircularEventQueue::CircularEventQueue(GLFWwindow* const t_glfWwindow, const size_t t_maxSize)
    : m_glfWwindow{ t_glfWwindow }
    , m_maxSize{ t_maxSize }
{
    SG_OGL_CORE_ASSERT(m_glfWwindow, "[CircularEventQueue::CircularEventQueue()] Null pointer.");
    glfwSetWindowUserPointer(m_glfWwindow, this);
}

sg::ogl::event::CircularEventQueue::~CircularEventQueue() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[CircularEventQueue::~CircularEventQueue()] Execute the CircularEventQueue destructor.");
}

//-------------------------------------------------
// Getter && Setter
//-------------------------------------------------

void sg::ogl::event::CircularEventQueue::SetCallbacks() const
{
    glfwSetWindowPosCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_xPos, int t_yPos)
        {
            PositionCategory e;
            e.eventType = EventType::WINDOW_MOVED;
            e.window = t_window;
            e.xPos = t_xPos;
            e.yPos = t_yPos;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetWindowSizeCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_width, int t_height)
        {
            SizeCategory e;
            e.eventType = EventType::WINDOW_RESIZED;
            e.window = t_window;
            e.width = t_width;
            e.height = t_height;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetWindowCloseCallback(m_glfWwindow,[](GLFWwindow* t_window)
        {
            SwitchCategory e;
            e.eventType = EventType::WINDOW_CLOSED;
            e.window = t_window;
            e.value = GLFW_TRUE;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetWindowRefreshCallback(m_glfWwindow,[](GLFWwindow* t_window)
        {
            SwitchCategory e;
            e.eventType = EventType::WINDOW_REFRESH;
            e.window = t_window;
            e.value = GLFW_TRUE;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetWindowFocusCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_focus)
        {
            SwitchCategory e;
            e.eventType = t_focus ? EventType::WINDOW_FOCUSED : EventType::WINDOW_DEFOCUSED;
            e.window = t_window;
            e.value = t_focus;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetWindowIconifyCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_iconified)
        {
            SwitchCategory e;
            e.eventType = t_iconified ? EventType::WINDOW_ICONIFIED : EventType::WINDOW_UNICONIFIED;
            e.window = t_window;
            e.value = t_iconified;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetFramebufferSizeCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_width, int t_height)
        {
            SizeCategory e;
            e.eventType = EventType::FRAMEBUFFER_RESIZED;
            e.window = t_window;
            e.width = t_width;
            e.height = t_height;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetMouseButtonCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_button, int t_action, int t_mods)
        {
            MouseCategory e;

            if (t_action == GLFW_PRESS)
            {
                e.eventType = EventType::BUTTON_PRESSED;
            }
            else
            {
                e.eventType = EventType::BUTTON_RELEASED;
            }

            e.window = t_window;
            e.button = t_button;
            e.mods = t_mods;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetCursorPosCallback(m_glfWwindow,[](GLFWwindow* t_window, double t_xPos, double t_yPos)
        {
            UseDeviceCategory e;
            e.eventType = EventType::CURSOR_MOVED;
            e.window = t_window;
            e.xPos = t_xPos;
            e.yPos = t_yPos;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetCursorEnterCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_entered)
        {
            SwitchCategory e;
            e.eventType = t_entered ? EventType::CURSOR_ENTERED : EventType::CURSOR_LEFT;
            e.window = t_window;
            e.value = t_entered;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetScrollCallback(m_glfWwindow,[](GLFWwindow* t_window, double t_xOffset, double t_yOffset)
        {
            UseDeviceCategory e;
            e.eventType = EventType::SCROLLED;
            e.window = t_window;
            e.xPos = t_xOffset;
            e.yPos = t_yOffset;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    glfwSetKeyCallback(m_glfWwindow,[](GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods)
        {
            KeyboardCategory e;

            if (t_action == GLFW_PRESS)
            {
                e.eventType = EventType::KEY_PRESSED;
            }
            else if (t_action == GLFW_RELEASE)
            {
                e.eventType = EventType::KEY_RELEASED;
            }
            else if (t_action == GLFW_REPEAT)
            {
                e.eventType = EventType::KEY_REPEATED;
            }

            e.window = t_window;
            e.key = t_key;
            e.scancode = t_scancode;
            e.mods = t_mods;

            auto windowUserPtr{ static_cast<CircularEventQueue*>(glfwGetWindowUserPointer(t_window)) };
            windowUserPtr->Add(e);
        }
    );

    // added in Glfw 3.3
    // glfwSetWindowMaximizeCallback();
    // glfwSetWindowContentScaleCallback();
}
