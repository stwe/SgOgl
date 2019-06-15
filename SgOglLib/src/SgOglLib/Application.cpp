#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "Color.h"
#include "State.h"

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application] Execute the Application constructor.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);

    m_window = std::make_unique<Window>(this);
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application] Execute the Application destructor.");

    CleanUp();
}

const sg::ogl::WindowOptions& sg::ogl::Application::GetWindowOptions() const noexcept
{
    return m_windowOptions;
}

sg::ogl::WindowOptions& sg::ogl::Application::GetWindowOptions() noexcept
{
    return m_windowOptions;
}

const sg::ogl::ProjectionOptions& sg::ogl::Application::GetProjectionOptions() const noexcept
{
    return m_projectionOptions;
}

sg::ogl::ProjectionOptions& sg::ogl::Application::GetProjectionOptions() noexcept
{
    return m_projectionOptions;
}

void sg::ogl::Application::Run()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Run()] Application is started.");

    Init();

    while (m_window->WindowIsNotClosed())
    {
        Window::SetClearColor(Color::CornflowerBlue());
        Window::Clear();

        if (PeekState())
        {
            PeekState()->Input();
            PeekState()->Update(0.0f);
            PeekState()->Render(0.0f);
        }

        m_window->Update();
    }
}

void sg::ogl::Application::PushState(State* t_state)
{
    t_state->SetApplication(this);
    m_states.push(t_state);
}

void sg::ogl::Application::PopState()
{
    // Top() returns a reference to the top element.
    delete m_states.top();

    // Remove top element.
    m_states.pop();
}

void sg::ogl::Application::ChangeState(State* t_state)
{
    if (!m_states.empty())
    {
        PopState();
    }

    PushState(t_state);
}

sg::ogl::State* sg::ogl::Application::PeekState()
{
    if (m_states.empty())
    {
        return nullptr;
    }

    return m_states.top();
}

void sg::ogl::Application::Init()
{
    m_window->Init();
}

void sg::ogl::Application::CleanUp()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up Application...");

    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up States.");
    while (!m_states.empty())
    {
        PopState();
    }

    m_window->CleanUp();
}
