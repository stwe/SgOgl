#pragma once

#include <memory>
#include <stack>
#include "Core.h"
#include "Config.h"

namespace sg::ogl::resource
{
    class ShaderManager;
}

namespace sg::ogl
{
    class Window;
    class State;

    class SG_OGL_API Application
    {
    public:
        using WindowUniquePtr = std::unique_ptr<Window>;
        using ShaderManagerUniquePtr = std::unique_ptr<resource::ShaderManager>;
        using States = std::stack<State*>;

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
        States m_states;

        //-------------------------------------------------
        // States
        //-------------------------------------------------

        void PushState(State* t_state);
        void PopState();
        void ChangeState(State* t_state);

        State* PeekState();

    private:
        WindowOptions m_windowOptions{};
        ProjectionOptions m_projectionOptions{};

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp();
    };

    //-------------------------------------------------
    // Client
    //-------------------------------------------------

    std::unique_ptr<Application> create_application();
}
