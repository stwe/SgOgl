#include "Application.h"
#include "Log.h"

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application] Execute the Application constructor.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application] Execute the Application destructor.");
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
}

void sg::ogl::Application::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up Application...");
}

void sg::ogl::Application::Init()
{

}
