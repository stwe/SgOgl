#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "state/State.h"
#include "state/StateStack.h"
#include "resource/ShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "input/MouseInput.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::DeleteWindow::operator()(Window* t_window) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteWindow::operator()] Delete Window.");
    delete t_window;
}

void sg::ogl::DeleteShaderManager::operator()(resource::ShaderManager* t_shaderManager) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteShaderManager::operator()] Delete ShaderManager.");
    delete t_shaderManager;
}

void sg::ogl::DeleteTextureManager::operator()(resource::TextureManager* t_textureManager) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteTextureManager::operator()] Delete TextureManager.");
    delete t_textureManager;
}

void sg::ogl::DeleteStateStack::operator()(state::StateStack* t_stateStack) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteStateStack::operator()] Delete StateStack.");
    delete t_stateStack;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application()] Execute the Application constructor.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application()] Execute the Application destructor.");
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
    ClientInit();
    GameLoop();
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
// Init && GameLoop
//-------------------------------------------------

void sg::ogl::Application::CoreInit()
{
    m_window.reset(new Window{ this });
    SG_OGL_CORE_ASSERT(m_window, "[Application::CoreInit()] Null pointer.")
    m_window->Init();

    m_shaderManager.reset(new resource::ShaderManager);
    SG_OGL_CORE_ASSERT(m_shaderManager, "[Application::CoreInit()] Null pointer.")

    m_textureManager.reset(new resource::TextureManager);
    SG_OGL_CORE_ASSERT(m_textureManager, "[Application::CoreInit()] Null pointer.")

    m_stateStack.reset(new state::StateStack{ state::State::Context{ *m_window, *m_shaderManager, *m_textureManager} } );
    SG_OGL_CORE_ASSERT(m_stateStack, "[Application::CoreInit()] Null pointer.")

    input::MouseInput::Init(*m_window);
}

void sg::ogl::Application::ClientInit()
{
    RegisterStates();
    Init();
}

void sg::ogl::Application::GameLoop()
{
    while (m_window->WindowIsNotClosed())
    {
        Input();
        Update(0.0f);
        Render();

        // m_window->Update():
        //     (1) Swap front and back buffers: glfwSwapBuffers(m_windowHandle)
        //     (2) Process events: glfwPollEvents()
        m_window->Update();
    }
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::Application::Input()
{
    input::MouseInput::GetInstance().Input();
    m_stateStack->Input();
}

void sg::ogl::Application::Update(const float t_dt)
{
    m_stateStack->Update(t_dt);
}

void sg::ogl::Application::Render()
{
    Window::Clear();

    m_stateStack->Render();
}
