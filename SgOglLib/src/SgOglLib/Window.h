#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
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

        const glm::mat4& GetProjectionMatrix() const noexcept;
        const glm::mat4& GetOrthographicProjectionMatrix() const noexcept;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        bool WindowIsNotClosed() const;
        bool IsKeyPressed(int t_keyCode) const;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update() const;
        void UpdateProjectionMatrix();
        void UpdateOrthographicProjectionMatrix();

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

        /**
         * @brief The perspective projection matrix.
         *        Updated with UpdateProjectionMatrix().
         */
        glm::mat4 m_projectionMatrix{ glm::mat4(1.0f) };

        /**
         * @brief The orthographic projection matrix.
         *        Updated with UpdateOrthographicProjectionMatrix().
         */
        glm::mat4 m_orthographicProjectionMatrix{ glm::mat4(1.0f) };
    };
}
