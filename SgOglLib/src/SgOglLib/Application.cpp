#include <sstream>

#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "OpenGl.h"
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

void sg::ogl::DeleteMouseInput::operator()(input::MouseInput* t_mouseInput) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteCircularEventQueue::operator()] Delete MouseInput.");
    delete t_mouseInput;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application()] Create Application.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application()] Destruct Application.");
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

sg::ogl::Application::WindowUniquePtr& sg::ogl::Application::GetWindow() noexcept
{
    return m_window;
}

sg::ogl::Application::ShaderManagerUniquePtr& sg::ogl::Application::GetShaderManager() noexcept
{
    return m_shaderManager;
}

sg::ogl::Application::TextureManagerUniquePtr& sg::ogl::Application::GetTextureManager() noexcept
{
    return m_textureManager;
}

sg::ogl::Application::MouseInputUniquePtr& sg::ogl::Application::GetMouseInput() noexcept
{
    return m_mouseInput;
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

    m_stateStack.reset(new state::StateStack{ this });
    SG_OGL_CORE_ASSERT(m_stateStack, "[Application::CoreInit()] Null pointer.")

    m_circularEventQueue.reset(new event::CircularEventQueue {m_window->GetWindowHandle(), 1024});
    SG_OGL_CORE_ASSERT(m_circularEventQueue, "[Application::CoreInit()] Null pointer.")

    m_mouseInput.reset(new input::MouseInput);
    SG_OGL_CORE_ASSERT(m_mouseInput, "[Application::CoreInit()] Null pointer.")

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
    static constexpr auto FPS{ 1.0 / 60.0 };

    auto lastTime{ glfwGetTime() };
    auto timer{ lastTime };

    auto deltaTime = 0.0;
    auto nowTime = 0.0;

    auto frames = 0;
    auto updates = 0;

    while (m_window->WindowIsNotClosed())
    {
        // measure time
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / FPS;
        lastTime = nowTime;

        Input(*m_circularEventQueue);

        // only update at 60 frames/s
        while (deltaTime >= 1.0)
        {
            Update(FPS, *m_circularEventQueue);
            updates++;
            deltaTime--;
        }

        // render at maximum possible frames
        Render();
        frames++;

        // reset after one second
        if (glfwGetTime() - timer > 1.0)
        {
            timer++;

            if (m_windowOptions.printFrameRate)
            {
                std::stringstream ss;
#ifdef _DEBUG
                ss << m_windowOptions.title << " [DEBUG BUILD] " << "   |   Fps: " << frames << "   |   Updates: " << updates;
#else
                ss << m_windowOptions.title << " [RELEASE BUILD] " << "   |   Fps: " << frames << "   |   Updates: " << updates;
#endif
                glfwSetWindowTitle(m_window->GetWindowHandle(), ss.str().c_str());
            }

            updates = 0;
            frames = 0;
        }

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
    // handle core input
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
        [this](event::SwitchCategory& t_event)
        {
                m_mouseInput->SetInWindow(t_event.value == 1);
        },
        [this](event::UseDeviceCategory& t_event)
        {
                m_mouseInput->SetCurrentPosition(glm::ivec2(static_cast<int>(t_event.xPos), static_cast<int>(t_event.yPos)));
        },
        [this](event::MouseCategory& t_event)
        {
            if (t_event.eventType == event::EventType::BUTTON_PRESSED)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_1)
                {
                    m_mouseInput->SetLeftButtonPressed(true);
                }

                if (t_event.button == GLFW_MOUSE_BUTTON_2)
                {
                    m_mouseInput->SetRightButtonPressed(true);
                }
            }
            if (t_event.eventType == event::EventType::BUTTON_RELEASED)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_1)
                {
                    m_mouseInput->SetLeftButtonPressed(false);
                }

                if (t_event.button == GLFW_MOUSE_BUTTON_2)
                {
                    m_mouseInput->SetRightButtonPressed(false);
                }
            }
        },
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

    // handle states input
    m_stateStack->Input();
}

void sg::ogl::Application::Update(const double t_dt, event::CircularEventQueue& t_circularEventQueue)
{
    // handle core update
    m_mouseInput->Update();

    // handle states update
    m_stateStack->Update(t_dt);
}

void sg::ogl::Application::Render()
{
    Window::Clear();

    // handle states render
    m_stateStack->Render();
}
