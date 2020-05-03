// This file is part of the SgOgl package.
// 
// Filename: EventCategories.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

struct GLFWwindow;

namespace sg::ogl::event
{
    //-------------------------------------------------
    // Event Types
    //-------------------------------------------------

    enum class EventType
    {
        // Default
        NONE,

        // Keyboard input
        KEY_PRESSED,
        KEY_REPEATED,
        KEY_RELEASED,

        // Mouse input
        BUTTON_PRESSED,
        BUTTON_RELEASED,
        CURSOR_MOVED,
        CURSOR_ENTERED,
        CURSOR_LEFT,
        SCROLLED,

        // Window events
        WINDOW_MOVED,
        WINDOW_RESIZED,
        WINDOW_CLOSED,
        WINDOW_REFRESH,
        WINDOW_FOCUSED,
        WINDOW_DEFOCUSED,
        WINDOW_ICONIFIED,
        WINDOW_UNICONIFIED,

        // Framebuffer size
        FRAMEBUFFER_RESIZED,
    };

    //-------------------------------------------------
    // Event Categories
    //-------------------------------------------------

    /**
     * @brief Using in glfwSetWindowPosCallback.
     */
    struct PositionCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int xPos{ 0 };
        int yPos{ 0 };
    };

    /**
     * @brief Using in glfwSetWindowSizeCallback and glfwSetFramebufferSizeCallback.
     */
    struct SizeCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int width{ 0 };
        int height{ 0 };
    };

    /**
     * @brief Using in glfwSetWindowCloseCallback, glfwSetWindowRefreshCallback,
     *                 glfwSetWindowFocusCallback, glfwSetWindowIconifyCallback and
     *                 glfwSetCursorEnterCallback.
     */
    struct SwitchCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int value{ 0 }; // GLFW_TRUE = 1 or GLFW_FALSE = 0
    };

    /**
     * @brief Using in glfwSetCursorPosCallback and glfwSetScrollCallback.
     */
    struct UseDeviceCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        double xPos{ 0.0 };
        double yPos{ 0.0 };
    };

    /**
     * @brief Using in glfwSetMouseButtonCallback.
     */
    struct MouseCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int button{ 0 };
        int mods{ 0 };
    };

    /**
     * @brief Using in glfwSetKeyCallback.
     */
    struct KeyboardCategory
    {
        EventType eventType{ EventType::NONE };
        GLFWwindow* window{ nullptr };
        int key{ 0 };
        int scancode{ 0 };
        int mods{ 0 };
    };
}
