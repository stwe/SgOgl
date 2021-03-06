// This file is part of the SgOgl package.
// 
// Filename: Application.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>
#include <entt/entt.hpp>
#include <string>
#include "Config.h"

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
    class ModelManager;
}

namespace sg::ogl::state
{
    class StateStack;
}

namespace sg::ogl::event
{
    class CircularEventQueue;
}

namespace sg::ogl::input
{
    class MouseInput;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl
{
    class Window;

    struct DeleteWindow
    {
        void operator()(Window* t_window) const;
    };

    struct DeleteShaderManager
    {
        void operator()(resource::ShaderManager* t_shaderManager) const;
    };

    struct DeleteTextureManager
    {
        void operator()(resource::TextureManager* t_textureManager) const;
    };

    struct DeleteModelManager
    {
        void operator()(resource::ModelManager* t_modelManager) const;
    };

    struct DeleteStateStack
    {
        void operator()(state::StateStack* t_stateStack) const;
    };

    struct DeleteCircularEventQueue
    {
        void operator()(event::CircularEventQueue* t_circularEventQueue) const;
    };

    struct DeleteMouseInput
    {
        void operator()(input::MouseInput* t_mouseInput) const;
    };

    class Application
    {
    public:
        using WindowUniquePtr = std::unique_ptr<Window, DeleteWindow>;
        using ShaderManagerUniquePtr = std::unique_ptr<resource::ShaderManager, DeleteShaderManager>;
        using TextureManagerUniquePtr = std::unique_ptr<resource::TextureManager, DeleteTextureManager>;
        using ModelManagerUniquePtr = std::unique_ptr<resource::ModelManager, DeleteModelManager>;
        using StateStackUniquePtr = std::unique_ptr<state::StateStack, DeleteStateStack>;
        using CircularEventQueueUniquePtr = std::unique_ptr<event::CircularEventQueue, DeleteCircularEventQueue>;
        using MouseInputUniquePtr = std::unique_ptr<input::MouseInput, DeleteMouseInput>;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        /**
         * @brief The core class of the EnTT Ecs.
         *        It stores entities and arranges pools of
         *        components on a per request basis.
         */
        entt::registry registry;

        scene::Scene* currentScene{ nullptr };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Application() = delete;

        explicit Application(const std::string& t_configFileName);

        Application(const Application& t_other) = delete;
        Application(Application&& t_other) noexcept = delete;
        Application& operator=(const Application& t_other) = delete;
        Application& operator=(Application&& t_other) noexcept = delete;

        virtual ~Application() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        std::string GetLibResFolder() const;

        const WindowOptions& GetWindowOptions() const noexcept;
        WindowOptions& GetWindowOptions() noexcept;

        const ProjectionOptions& GetProjectionOptions() const noexcept;
        ProjectionOptions& GetProjectionOptions() noexcept;

        Window& GetWindow() noexcept;
        resource::ShaderManager& GetShaderManager() noexcept;
        resource::TextureManager& GetTextureManager() noexcept;
        resource::ModelManager& GetModelManager() noexcept;
        state::StateStack& GetStateStack() noexcept;
        event::CircularEventQueue& GetCircularEventQueue() noexcept;
        input::MouseInput& GetMouseInput() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:
        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        virtual void RegisterStates();
        virtual void Init();

    private:
        std::string m_libResFolder;
        WindowOptions m_windowOptions{};
        ProjectionOptions m_projectionOptions{};

        WindowUniquePtr m_window;
        ShaderManagerUniquePtr m_shaderManager;
        TextureManagerUniquePtr m_textureManager;
        ModelManagerUniquePtr m_modelManager;
        StateStackUniquePtr m_stateStack;
        CircularEventQueueUniquePtr m_circularEventQueue;
        MouseInputUniquePtr m_mouseInput;

        //-------------------------------------------------
        // Init && GameLoop
        //-------------------------------------------------

        void CoreInit();
        void ClientInit();
        void GameLoop();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input(event::CircularEventQueue& t_circularEventQueue);
        void Update(double t_dt, event::CircularEventQueue& t_circularEventQueue);
        void Render();
    };

    //-------------------------------------------------
    // Client
    //-------------------------------------------------

    std::unique_ptr<Application> create_application();
}
