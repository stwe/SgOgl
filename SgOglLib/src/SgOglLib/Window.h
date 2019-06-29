#pragma once

#include <memory>
#include "Core.h"

struct GLFWwindow;

namespace sg::ogl
{
    class Application;
    struct Color;

    struct GlfwDeleteWindow
    {
        void operator()(GLFWwindow* t_window) const;
    };

    class SG_OGL_API Window
    {
    public:
        using GlfWwindowUniquePtr = std::unique_ptr<GLFWwindow, GlfwDeleteWindow>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Window() = delete;

        explicit Window(Application* t_application);

        Window(const Window& t_other) = delete;
        Window(Window&& t_other) noexcept = delete;
        Window& operator=(const Window& t_other) = delete;
        Window& operator=(Window&& t_other) noexcept = delete;

        ~Window() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        GLFWwindow* GetWindowHandle() const;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        bool WindowIsNotClosed() const;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update() const;

        //-------------------------------------------------
        // OpenGL
        //-------------------------------------------------

        static void SetClearColor(const Color& t_color);
        static void Clear();
        static void EnableFaceCulling();
        static void DisableFaceCulling();
        static void EnableBlending();
        static void DisableBlending();
        void RestoreInitialGlStates() const;

    protected:

    private:
        Application* m_application{ nullptr };
        GlfWwindowUniquePtr m_windowHandle;
    };
}
