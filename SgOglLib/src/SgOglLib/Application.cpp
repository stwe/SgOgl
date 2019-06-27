#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "Color.h"
#include "state/State.h"
#include "state/StateStack.h"
#include "resource/ShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application] Execute the Application constructor.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application] Execute the Application destructor.");

    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

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

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::ogl::Application::Run()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Run()] Application is started.");

    CoreInit();

    while (m_window->WindowIsNotClosed())
    {
        Update(0.0f);
        Render();

        m_window->Update();
    }
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::ogl::Application::RegisterStates()
{
}

void sg::ogl::Application::Init()
{
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::Application::CoreInit()
{
    m_window = std::make_unique<Window>(this);
    SG_OGL_CORE_ASSERT(m_window, "[Application::CoreInit()] Null pointer.")
    m_window->Init();

    m_shaderManager = std::make_unique<resource::ShaderManager>();
    SG_OGL_CORE_ASSERT(m_shaderManager, "[Application::CoreInit()] Null pointer.")

    m_textureManager = std::make_unique<resource::TextureManager>();
    SG_OGL_CORE_ASSERT(m_textureManager, "[Application::CoreInit()] Null pointer.")

    m_stateStack = std::make_unique<state::StateStack>(state::State::Context(*m_window, *m_shaderManager, *m_textureManager));
    SG_OGL_CORE_ASSERT(m_stateStack, "[Application::CoreInit()] Null pointer.")

    RegisterStates();
    Init();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::Application::Update(const float t_dt)
{
    m_stateStack->Update(t_dt);
}

void sg::ogl::Application::Render()
{
    Window::SetClearColor(Color::CornflowerBlue());
    Window::Clear();

    m_stateStack->Render();
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::Application::CleanUp()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up Application...");

    m_window->CleanUp();
}
