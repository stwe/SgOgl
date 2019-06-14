#include "Window.h"
#include "OpenGl.h"
#include "Application.h"
#include "SgException.h"
#include "Color.h"

sg::ogl::Window::Window(Application* t_application)
    : m_application{ t_application }
{}

GLFWwindow* sg::ogl::Window::GetWindowHandle() const
{
    return m_windowHandle;
}

void sg::ogl::Window::Init()
{
    SG_OGL_CORE_ASSERT(m_application, "[Window::Init()] Application missing.")

    // Setup an error callback.
    glfwSetErrorCallback
            (
                    [](int t_error, const char* t_description)
                    {
                        SG_OGL_CORE_LOG_ERROR("GLFW Error ({}) {}", t_error, t_description);
                    }
            );

    // Initialize GLFW.
    if (!glfwInit())
    {
        SG_OGL_EXCEPTION("[Window::Init()] Unable to initialize GLFW.");
    }

    auto& windowOptions{ m_application->GetWindowOptions() };
    auto& projectionOptions{ m_application->GetProjectionOptions() };

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // todo
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // todo

    if (windowOptions.debugContext)
    {
        SG_OGL_CORE_LOG_WARN("WARNING: Using OpenGL in debug context!");
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }

    if (windowOptions.compatibleProfile)
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }
    else
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    SG_OGL_CORE_ASSERT(projectionOptions.width > 0, "The width should be greater than 0.")
    SG_OGL_CORE_ASSERT(projectionOptions.height > 0, "The height should be greater than 0.")

    // Create the window.
    m_windowHandle = glfwCreateWindow(projectionOptions.width, projectionOptions.height, windowOptions.title.c_str(), nullptr, nullptr);
    if (!m_windowHandle)
    {
        SG_OGL_EXCEPTION("[Window::Init()] Failed to create the GLFW window.");
    }

    // Get the resolution of the primary monitor.
    const auto* primaryMonitor{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };

    // Center our window.
    glfwSetWindowPos(m_windowHandle, (primaryMonitor->width - projectionOptions.width) / 2, (primaryMonitor->height - projectionOptions.height) / 2);

    // Make the OpenGL context current.
    glfwMakeContextCurrent(m_windowHandle);

    // Initialize GLEW.
    const auto err{ glewInit() };
    if (err != GLEW_OK)
    {
        SG_OGL_EXCEPTION("[Window::Init()] Unable to initialize GLEW." + std::string(reinterpret_cast<const char*>(glewGetErrorString(err))));
    }

    // Registering OpenGL Debug Callback.
    if (windowOptions.debugContext)
    {
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(gl_debug_output, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            //glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
        }
    }

    // Print out some informations about the graphics drivers.
    SG_OGL_CORE_LOG_INFO("OpenGL version: {}", glGetString(GL_VERSION));
    SG_OGL_CORE_LOG_INFO("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    SG_OGL_CORE_LOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    SG_OGL_CORE_LOG_INFO("Renderer: {}", glGetString(GL_RENDERER));

    // Set the user pointer.
    glfwSetWindowUserPointer(m_windowHandle, this);

    // Set a framebuffer size callback.
    glfwSetFramebufferSizeCallback
            (
                    m_windowHandle,
                    [](GLFWwindow* t_window, int t_width, int t_height)
                    {
                        static auto* win{ static_cast<Window*>(glfwGetWindowUserPointer(t_window)) };

                        auto& projection{ win->m_application->GetProjectionOptions() };

                        // Update width && height.
                        projection.width = t_width;
                        projection.height = t_height;

                        // Update viewport.
                        glViewport(0, 0, projection.width, projection.height);

                        // todo
                        // Update projection matrix.
                        //win->UpdateProjectionMatrix();
                        //win->UpdateOrthographicProjectionMatrix();
                    }
            );

    // Setup a key callback.
    glfwSetKeyCallback(
            m_windowHandle,
            [](GLFWwindow* t_window, int t_key, int t_scancode, int t_action, int t_mods)
            {
                static auto* win{ static_cast<Window*>(glfwGetWindowUserPointer(t_window)) };

                auto& windowOptions{ win->m_application->GetWindowOptions() };

                if (t_key == GLFW_KEY_ESCAPE && t_action == GLFW_RELEASE)
                {
                    // We will detect this in the game loop.
                    glfwSetWindowShouldClose(t_window, GLFW_TRUE);
                }

                if (t_key == GLFW_KEY_T && t_action == GLFW_RELEASE)
                {
                    windowOptions.showTriangles = !windowOptions.showTriangles;
                    windowOptions.showTriangles ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                if (t_key == GLFW_KEY_C && t_action == GLFW_RELEASE)
                {
                    windowOptions.faceCulling = !windowOptions.faceCulling;
                    windowOptions.faceCulling ? EnableFaceCulling() : DisableFaceCulling();
                }
            }
    );

    // Make the window visible.
    glfwShowWindow(m_windowHandle);

    // If showTriangles, draw a surface with outlined polygons.
    if (windowOptions.showTriangles)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Set default states.
    RestoreInitialGlStates();

    // Antialiasing.
    if (windowOptions.antialiasing)
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }
}

bool sg::ogl::Window::WindowIsNotClosed() const
{
    return glfwWindowShouldClose(m_windowHandle) == 0;
}

void sg::ogl::Window::Update() const
{
    glfwSwapBuffers(m_windowHandle);
    glfwPollEvents();
}

void sg::ogl::Window::SetClearColor(const Color& t_color)
{
    glClearColor(t_color.r / 255.0f, t_color.g / 255.0f, t_color.b / 255.0f, t_color.a / 255.0f);
}

void sg::ogl::Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void sg::ogl::Window::EnableFaceCulling()
{
    // On a freshly created OpenGL Context, the default front face is GL_CCW.
    // All the faces that are not front-faces are discarded.
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    SG_OGL_CORE_LOG_DEBUG("[Window::EnableFaceCulling()] Face culling enabled (front = ccw).");
}

void sg::ogl::Window::DisableFaceCulling()
{
    glDisable(GL_CULL_FACE);

    SG_OGL_CORE_LOG_DEBUG("[Window::DisableFaceCulling()] Face culling disabled.");
}

void sg::ogl::Window::EnableBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SG_OGL_CORE_LOG_DEBUG("[Window::EnableBlending()] Blending enabled.");
}

void sg::ogl::Window::DisableBlending()
{
    glDisable(GL_BLEND);

    SG_OGL_CORE_LOG_DEBUG("[Window::DisableBlending()] Blending disabled.");
}

void sg::ogl::Window::RestoreInitialGlStates() const
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    auto& windowOptions{ m_application->GetWindowOptions() };
    windowOptions.faceCulling ? EnableFaceCulling() : DisableFaceCulling();
}

void sg::ogl::Window::CleanUp()
{
    SG_OGL_CORE_LOG_DEBUG("[Window::CleanUp()] Cleaning up Window.");

    glfwTerminate();
}
