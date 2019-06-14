#pragma once

#include "Core.h"

struct GLFWwindow;

namespace sg::ogl
{
    class Application;
    class Color;

    class SG_OGL_API Window
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Window() = delete;

        explicit Window(Application* t_application);

        Window(const Window& t_other) = delete;
        Window(Window&& t_other) noexcept = delete;
        Window& operator=(const Window& t_other) = delete;
        Window& operator=(Window&& t_other) noexcept = delete;

        ~Window() noexcept = default;

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

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        static void CleanUp();

    protected:

    private:
        /**
         * @brief Pointer to store the parent Application.
         */
        Application* m_application{ nullptr };

        /**
         * @brief Pointer to store the GLFWwindow.
         */
        GLFWwindow* m_windowHandle{ nullptr };
    };
}
