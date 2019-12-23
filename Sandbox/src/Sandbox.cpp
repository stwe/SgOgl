// This file is part of the SgOgl package.
// 
// Filename: Sandbox.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "SgOgl.h"
#include "SgOglEntryPoint.h"
#include "SponzaPlaygroundState.h"
#include "TestState.h"

class Sandbox final : public sg::ogl::Application
{
public:
    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    Sandbox() = delete;

    explicit Sandbox(const std::string& t_configFileName)
        : Application{ t_configFileName }
    {
        SG_OGL_LOG_DEBUG("[Sandbox::Sandbox()] Create Sandbox.");
    }

    Sandbox(const Sandbox& t_other) = delete;
    Sandbox(Sandbox&& t_other) noexcept = delete;
    Sandbox& operator=(const Sandbox& t_other) = delete;
    Sandbox& operator=(Sandbox&& t_other) noexcept = delete;

    ~Sandbox() noexcept override
    {
        SG_OGL_LOG_DEBUG("[Sandbox::~Sandbox()] Destruct Sandbox.");
    }

protected:
    //-------------------------------------------------
    // Override
    //-------------------------------------------------

    void RegisterStates() override
    {
        SG_OGL_LOG_DEBUG("[Sandbox::RegisterStates()] Register State: SponzaPlaygroundState as Game.");
        GetStateStack().RegisterState<SponzaPlaygroundState>(sg::ogl::state::GAME);

        //SG_OGL_LOG_DEBUG("[Sandbox::RegisterStates()] Register State: TestState as Game.");
        //GetStateStack().RegisterState<TestState>(sg::ogl::state::GAME);
    }

    void Init() override
    {
        SG_OGL_LOG_DEBUG("[Sandbox::Init()] Init (Push) Game State.");
        GetStateStack().PushState(sg::ogl::state::GAME);
    }

private:

};

//-------------------------------------------------
// EntryPoint
//-------------------------------------------------

std::unique_ptr<sg::ogl::Application> sg::ogl::create_application()
{
    return std::make_unique<Sandbox>("res/config/Config.xml");
}
