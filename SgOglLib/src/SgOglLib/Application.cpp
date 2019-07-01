#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "OpenGl.h"
#include "state/State.h"
#include "state/StateStack.h"
#include "resource/ShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "input/MouseInput.h"
#include "event/CircularEventQueue.h"

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

void sg::ogl::DeleteCircularEventQueue::operator()(event::CircularEventQueue* t_circularEventQueue) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteCircularEventQueue::operator()] Delete CircularEventQueue.");
    delete t_circularEventQueue;
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

    m_stateStack.reset(new state::StateStack{ state::State::Context{ *m_window, *m_shaderManager, *m_textureManager} });
    SG_OGL_CORE_ASSERT(m_stateStack, "[Application::CoreInit()] Null pointer.")

    m_circularEventQueue.reset(new event::CircularEventQueue {m_window->GetWindowHandle(), 1024});
    SG_OGL_CORE_ASSERT(m_circularEventQueue, "[Application::CoreInit()] Null pointer.")

    // todo
    input::MouseInput::Init(*m_window);

    // replace all callbacks
    m_circularEventQueue->SetCallbacks();
}

void sg::ogl::Application::ClientInit()
{
    RegisterStates();
    Init();
}

void sg::ogl::Application::GameLoop()
{
    static constexpr auto DT{ 1.0f / 60.0f };

    while (m_window->WindowIsNotClosed())
    {
        Input(*m_circularEventQueue);
        Update(DT, *m_circularEventQueue);
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

void sg::ogl::Application::Input(event::CircularEventQueue& t_circularEventQueue)
{
    // handle core events
    t_circularEventQueue.Update(event::EventVisitor(
        [](event::PositionCategory&) {},
        [this](event::SizeCategory& t_event)
        {
            if (t_event.eventType == event::EventType::FRAMEBUFFER_RESIZED ||
                t_event.eventType == event::EventType::WINDOW_RESIZED)
            {
                // Update width && height.
                m_projectionOptions.width = t_event.width;
                m_projectionOptions.height = t_event.height;

                // Update viewport.
                glViewport(0, 0, m_projectionOptions.width, m_projectionOptions.height);

                // Update projection matrix.
                m_window->UpdateProjectionMatrix();
                m_window->UpdateOrthographicProjectionMatrix();
            }
        },
        [](event::SwitchCategory&) {},
        [](event::UseDeviceCategory&) {},
        [](event::MouseCategory&) {},
        [this](event::KeyboardCategory& t_event)
        {
            // Exit
            if (t_event.eventType == event::EventType::KEY_PRESSED && t_event.key == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(t_event.window, GLFW_TRUE);
            }

            // Show Triangles
            if (t_event.eventType == event::EventType::KEY_PRESSED && t_event.key == GLFW_KEY_T)
            {
                m_windowOptions.showTriangles = !m_windowOptions.showTriangles;
                m_windowOptions.showTriangles ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            // Face Culling
            if (t_event.eventType == event::EventType::KEY_PRESSED && t_event.key == GLFW_KEY_C)
            {
                m_windowOptions.faceCulling = !m_windowOptions.faceCulling;
                m_windowOptions.faceCulling ? m_window->EnableFaceCulling() : m_window->DisableFaceCulling();
            }
        })
    );

    // todo
    input::MouseInput::GetInstance().Input();

    m_stateStack->Input();
}

void sg::ogl::Application::Update(const float t_dt, event::CircularEventQueue& t_circularEventQueue)
{
    m_stateStack->Update(t_dt);
}

void sg::ogl::Application::Render()
{
    Window::Clear();

    m_stateStack->Render();
}
