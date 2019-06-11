#pragma once

#include <memory>
#include "Core.h"

namespace sg::ogl
{
    class SG_OGL_API Application
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Application() = default;

        Application(const Application& t_other) = delete;
        Application(Application&& t_other) noexcept = delete;
        Application& operator=(const Application& t_other) = delete;
        Application& operator=(Application&& t_other) noexcept = delete;

        virtual ~Application() noexcept = default;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;

    protected:

    private:
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
