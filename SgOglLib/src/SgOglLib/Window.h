#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

struct GLFWwindow;

namespace sg::ogl
{
    class Application;

    struct GlfwDeleteWindow
    {
        void operator()(GLFWwindow* t_window) const;
    };

    class Window
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

        //-------------------------------------------------
        // Input
        //-------------------------------------------------

        bool IsKeyPressed(int t_keyCode) const;
        bool IsMouseButtonPressed(int t_button) const;
        glm::vec2 GetMousePosition() const;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update() const;
        void UpdateProjectionMatrix();
        void UpdateOrthographicProjectionMatrix();

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
