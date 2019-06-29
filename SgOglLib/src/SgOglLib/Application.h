#pragma once

#include <memory>
#include "Core.h"
#include "Config.h"

namespace sg::ogl::resource
{
    class ShaderManager;
    class TextureManager;
}

namespace sg::ogl::state
{
    class StateStack;
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

    struct DeleteStateStack
    {
        void operator()(state::StateStack* t_stateStack) const;
    };

    class SG_OGL_API Application
    {
    public:
        using WindowUniquePtr = std::unique_ptr<Window, DeleteWindow>;
        using ShaderManagerUniquePtr = std::unique_ptr<resource::ShaderManager, DeleteShaderManager>;
        using TextureManagerUniquePtr = std::unique_ptr<resource::TextureManager, DeleteTextureManager>;
        using StateStackUniquePtr = std::unique_ptr<state::StateStack, DeleteStateStack>;

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

        const WindowOptions& GetWindowOptions() const noexcept;
        WindowOptions& GetWindowOptions() noexcept;

        const ProjectionOptions& GetProjectionOptions() const noexcept;
        ProjectionOptions& GetProjectionOptions() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:
        WindowUniquePtr m_window;
        ShaderManagerUniquePtr m_shaderManager;
        TextureManagerUniquePtr m_textureManager;
        StateStackUniquePtr m_stateStack;

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        virtual void RegisterStates();
        virtual void Init();

    private:
        WindowOptions m_windowOptions{};
        ProjectionOptions m_projectionOptions{};

        //-------------------------------------------------
        // Init && GameLoop
        //-------------------------------------------------

        void CoreInit();
        void ClientInit();
        void GameLoop();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update(float t_dt);
        void Render();
    };

    //-------------------------------------------------
    // Client
    //-------------------------------------------------

    std::unique_ptr<Application> create_application();
}
