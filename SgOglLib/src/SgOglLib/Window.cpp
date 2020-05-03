#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "OpenGl.h"
#include "Application.h"
#include "SgOglException.h"
#include "Log.h"
#include "Core.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::GlfwDeleteWindow::operator()(GLFWwindow* t_window) const
{
    SG_OGL_CORE_LOG_DEBUG("[GlfwDeleteWindow::operator()] Destroying GLFW Window Context.");
    glfwDestroyWindow(t_window);
    glfwTerminate();
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Window::Window(Application* const t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[Window::Window()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[Window::Window()] Create Window.");
}

sg::ogl::Window::~Window() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Window::~Window()] Destruct Window.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

GLFWwindow* sg::ogl::Window::GetWindowHandle() const
{
    return m_windowHandle.get();
}

const glm::mat4& sg::ogl::Window::GetProjectionMatrix() const noexcept
{
    return m_projectionMatrix;
}

const glm::mat4& sg::ogl::Window::GetOrthographicProjectionMatrix() const noexcept
{
    return m_orthographicProjectionMatrix;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::Window::Init()
{
    SG_OGL_CORE_ASSERT(m_application, "[Window::Init()] Application missing.")

    // Setup an error callback.
    glfwSetErrorCallback([](int t_error, const char* t_description)
        {
            SG_OGL_CORE_LOG_ERROR("GLFW Error ({}) {}", t_error, t_description);
        }
    );

    // Initialize GLFW.
    SG_OGL_CORE_LOG_DEBUG("[Window::Init()] Initialize GLFW3.");
    if (!glfwInit())
    {
        throw SG_OGL_EXCEPTION("[Window::Init()] Unable to initialize GLFW.");
    }

    auto& windowOptions{ m_application->GetWindowOptions() };
    auto& projectionOptions{ m_application->GetProjectionOptions() };

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowOptions.glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowOptions.glMinor);

    if (windowOptions.debugContext)
    {
        SG_OGL_CORE_LOG_WARN("WARNING: Using OpenGL in Debug Context!");
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }

    if (windowOptions.compatibleProfile)
    {
        SG_OGL_CORE_LOG_WARN("WARNING: Using the OpenGL Compatibility Profile!");
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }
    else
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    SG_OGL_CORE_ASSERT(projectionOptions.width > 0, "The width should be greater than 0.")
    SG_OGL_CORE_ASSERT(projectionOptions.height > 0, "The height should be greater than 0.")

    // Create the glfw window.
    SG_OGL_CORE_LOG_DEBUG("[Window::Init()] Initialize GLFW3 Window. Width: {}, Height {}", projectionOptions.width, projectionOptions.height);
    m_windowHandle.reset(glfwCreateWindow(projectionOptions.width, projectionOptions.height, windowOptions.title.c_str(), nullptr, nullptr));
    if (!m_windowHandle)
    {
        throw SG_OGL_EXCEPTION("[Window::Init()] Failed to create the GLFW window.");
    }

    // Get the resolution of the primary monitor.
    const auto* primaryMonitor{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };
    if (!primaryMonitor)
    {
        throw SG_OGL_EXCEPTION("[Window::Init()] Unable to get the primary monitor.");
    }

    // Center our window.
    glfwSetWindowPos(GetWindowHandle(), (primaryMonitor->width - projectionOptions.width) / 2, (primaryMonitor->height - projectionOptions.height) / 2);

    // Make the OpenGL context current.
    glfwMakeContextCurrent(GetWindowHandle());

    // Initialize GLEW.
    SG_OGL_CORE_LOG_DEBUG("[Window::Init()] Initialize GLEW.");
    const auto err{ glewInit() };
    if (err != GLEW_OK)
    {
        throw SG_OGL_EXCEPTION("[Window::Init()] Unable to initialize GLEW." + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    // Registering OpenGL Debug Callback.
    if (windowOptions.debugContext)
    {
        int32_t flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            SG_OGL_CORE_LOG_DEBUG("[Window::Init()] Initialize OpenGL debug output.");

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(gl_debug_output, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Window::Init()] Unable to initialize the OpenGL debug output.");
        }
    }

    // Print out some informations about the graphics drivers.
    SG_OGL_CORE_LOG_INFO("OpenGL version: {}", glGetString(GL_VERSION));
    SG_OGL_CORE_LOG_INFO("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    SG_OGL_CORE_LOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    SG_OGL_CORE_LOG_INFO("Renderer: {}", glGetString(GL_RENDERER));

    // Make the window visible.
    glfwShowWindow(GetWindowHandle());

    // Enable Depth and Stencil buffer.
    OpenGl::EnableDepthAndStencilTesting();

    // Antialiasing.
    if (windowOptions.antialiasing)
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    // Set/Update the projection matrix.
    UpdateProjectionMatrix();
    UpdateOrthographicProjectionMatrix();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

bool sg::ogl::Window::WindowIsNotClosed() const
{
    return glfwWindowShouldClose(GetWindowHandle()) == 0;
}

//-------------------------------------------------
// Input
//-------------------------------------------------

bool sg::ogl::Window::IsKeyPressed(const int t_keyCode) const
{
    const auto state{ glfwGetKey(GetWindowHandle(), t_keyCode) };
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool sg::ogl::Window::IsMouseButtonPressed(const int t_button) const
{
    const auto state{ glfwGetMouseButton(GetWindowHandle(), t_button) };
    return state == GLFW_PRESS;
}

glm::vec2 sg::ogl::Window::GetMousePosition() const
{
    double xpos, ypos;
    glfwGetCursorPos(GetWindowHandle(), &xpos, &ypos);
    return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::Window::Update() const
{
    glfwSwapBuffers(GetWindowHandle());
    glfwPollEvents();
}

void sg::ogl::Window::UpdateProjectionMatrix()
{
    const auto& projectionOptions{ m_application->GetProjectionOptions() };
    m_projectionMatrix = glm::perspectiveFov
    (
        glm::radians(projectionOptions.fovDeg),
        static_cast<float>(projectionOptions.width),
        static_cast<float>(projectionOptions.height),
        projectionOptions.nearPlane,
        projectionOptions.farPlane
    );
}

void sg::ogl::Window::UpdateOrthographicProjectionMatrix()
{
    const auto& projectionOptions{ m_application->GetProjectionOptions() };
    m_orthographicProjectionMatrix = glm::ortho
    (
        0.0f,
        static_cast<float>(projectionOptions.width),
        0.0f,
        static_cast<float>(projectionOptions.height)
    );
}
