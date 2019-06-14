#pragma once

#include <memory>
#include "Core.h"
#include "Config.h"

namespace sg::ogl
{
    class Window;

    class SG_OGL_API Application
    {
    public:
        using WindowUniquePtr = std::unique_ptr<Window>;

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

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;

    protected:
        WindowUniquePtr m_window;

    private:
        WindowOptions m_windowOptions{};
        ProjectionOptions m_projectionOptions{};

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };

    //-------------------------------------------------
    // Client
    //-------------------------------------------------

    std::unique_ptr<Application> create_application();
}
