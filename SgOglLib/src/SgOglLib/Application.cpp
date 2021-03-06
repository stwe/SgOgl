// This file is part of the SgOgl package.
// 
// Filename: Application.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <sstream>
#include "Application.h"
#include "Core.h"
#include "Window.h"
#include "OpenGl.h"
#include "state/StateStack.h"
#include "resource/ShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "resource/ModelManager.h"
#include "input/MouseInput.h"
#include "event/CircularEventQueue.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::DeleteWindow::operator()(Window* t_window) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteWindow::operator()] Delete Window.");
    delete t_window;
}

void sg::ogl::DeleteShaderManager::operator()(resource::ShaderManager* t_shaderManager) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteShaderManager::operator()] Delete ShaderManager.");
    delete t_shaderManager;
}

void sg::ogl::DeleteTextureManager::operator()(resource::TextureManager* t_textureManager) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteTextureManager::operator()] Delete TextureManager.");
    delete t_textureManager;
}

void sg::ogl::DeleteModelManager::operator()(resource::ModelManager* t_modelManager) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteModelManager::operator()] Delete ModelManager.");
    delete t_modelManager;
}

void sg::ogl::DeleteStateStack::operator()(state::StateStack* t_stateStack) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteStateStack::operator()] Delete StateStack.");
    delete t_stateStack;
}

void sg::ogl::DeleteCircularEventQueue::operator()(event::CircularEventQueue* t_circularEventQueue) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteCircularEventQueue::operator()] Delete CircularEventQueue.");
    delete t_circularEventQueue;
}

void sg::ogl::DeleteMouseInput::operator()(input::MouseInput* t_mouseInput) const
{
    Log::SG_OGL_CORE_LOG_DEBUG("[DeleteCircularEventQueue::operator()] Delete MouseInput.");
    delete t_mouseInput;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Application::Application()] Create Application.");

    Config::LoadOptions(t_configFileName, m_libResFolder, m_windowOptions, m_projectionOptions);
}

sg::ogl::Application::~Application() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Application::~Application()] Destruct Application.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

std::string sg::ogl::Application::GetLibResFolder() const
{
    return m_libResFolder;
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

sg::ogl::Window& sg::ogl::Application::GetWindow() noexcept
{
    return *m_window;
}

sg::ogl::resource::ShaderManager& sg::ogl::Application::GetShaderManager() noexcept
{
    return *m_shaderManager;
}

sg::ogl::resource::TextureManager& sg::ogl::Application::GetTextureManager() noexcept
{
    return *m_textureManager;
}

sg::ogl::resource::ModelManager& sg::ogl::Application::GetModelManager() noexcept
{
    return *m_modelManager;
}

sg::ogl::state::StateStack& sg::ogl::Application::GetStateStack() noexcept
{
    return *m_stateStack;
}

sg::ogl::event::CircularEventQueue& sg::ogl::Application::GetCircularEventQueue() noexcept
{
    return *m_circularEventQueue;
}

sg::ogl::input::MouseInput& sg::ogl::Application::GetMouseInput() noexcept
{
    return *m_mouseInput;
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::ogl::Application::Run()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Application::Run()] Application is started.");

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
    SG_OGL_CORE_ASSERT(m_window, "[Application::CoreInit()] Null pointer.");
    m_window->Init();

    m_shaderManager.reset(new resource::ShaderManager(m_libResFolder));
    SG_OGL_CORE_ASSERT(m_shaderManager, "[Application::CoreInit()] Null pointer.");

    m_textureManager.reset(new resource::TextureManager);
    SG_OGL_CORE_ASSERT(m_textureManager, "[Application::CoreInit()] Null pointer.");

    m_modelManager.reset(new resource::ModelManager(this));
    SG_OGL_CORE_ASSERT(m_modelManager, "[Application::CoreInit()] Null pointer.");

    m_stateStack.reset(new state::StateStack{ this });
    SG_OGL_CORE_ASSERT(m_stateStack, "[Application::CoreInit()] Null pointer.");

    m_circularEventQueue.reset(new event::CircularEventQueue {m_window->GetWindowHandle(), 1024});
    SG_OGL_CORE_ASSERT(m_circularEventQueue, "[Application::CoreInit()] Null pointer.");

    m_mouseInput.reset(new input::MouseInput);
    SG_OGL_CORE_ASSERT(m_mouseInput, "[Application::CoreInit()] Null pointer.");

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
    // by default run at a constant rate of 60 frames per second
    // this results in a frametime of 0.016 (16ms)
    const auto frametime{ 1.0 / m_windowOptions.fps };

    auto lastTime{ glfwGetTime() };
    auto timer{ lastTime };

    auto deltaTime = 0.0;

    auto frames = 0;
    auto updates = 0;

    while (m_window->WindowIsNotClosed())
    {
        // measure time
        const auto nowTime{ glfwGetTime() };
        deltaTime += (nowTime - lastTime) / frametime;
        lastTime = nowTime;

        Input(*m_circularEventQueue);

        // only update at 60 frames per second (default)
        while (deltaTime >= 1.0)
        {
            Update(frametime, *m_circularEventQueue);
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
#ifdef SG_OGL_DEBUG_BUILD
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
            // todo
            if (t_event.eventType == event::EventType::SCROLLED)
            {
                m_mouseInput->SetScrolled(true);
                m_mouseInput->SetScrollOffset(glm::vec2(t_event.xPos, t_event.yPos));
            }

            m_mouseInput->SetCurrentPosition(glm::ivec2(static_cast<int>(t_event.xPos), static_cast<int>(t_event.yPos)));
        },
        [this](event::MouseCategory& t_event)
        {
            if (t_event.eventType == event::EventType::BUTTON_PRESSED)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_1)
                {
                    m_mouseInput->buttonStates.at(input::MouseInput::MouseButton::LEFT) = input::MouseInput::MouseState::PRESSED;
                }

                if (t_event.button == GLFW_MOUSE_BUTTON_2)
                {
                    m_mouseInput->buttonStates.at(input::MouseInput::MouseButton::RIGHT) = input::MouseInput::MouseState::PRESSED;
                }
            }
            if (t_event.eventType == event::EventType::BUTTON_RELEASED)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_1)
                {
                    m_mouseInput->buttonStates.at(input::MouseInput::MouseButton::LEFT) = input::MouseInput::MouseState::RELEASED;
                }

                if (t_event.button == GLFW_MOUSE_BUTTON_2)
                {
                    m_mouseInput->buttonStates.at(input::MouseInput::MouseButton::RIGHT) = input::MouseInput::MouseState::RELEASED;
                }
            }
        },
        [](event::KeyboardCategory& t_event)
        {
            // Exit
            if (t_event.eventType == event::EventType::KEY_PRESSED && t_event.key == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(t_event.window, GLFW_TRUE);
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
    // clear color, depth and stencil buffer
    OpenGl::Clear();

    // handle states render
    m_stateStack->Render();
}
