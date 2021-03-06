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
#include "ClientLog.h"
#include "new/NewState.h"

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
        ClientLog::Init();
        ClientLog::SG_SANDBOX_LOG_DEBUG("[Sandbox::Sandbox()] Create Sandbox.");
    }

    Sandbox(const Sandbox& t_other) = delete;
    Sandbox(Sandbox&& t_other) noexcept = delete;
    Sandbox& operator=(const Sandbox& t_other) = delete;
    Sandbox& operator=(Sandbox&& t_other) noexcept = delete;

    ~Sandbox() noexcept override
    {
        ClientLog::SG_SANDBOX_LOG_DEBUG("[Sandbox::~Sandbox()] Destruct Sandbox.");
    }

protected:
    //-------------------------------------------------
    // Override
    //-------------------------------------------------

    void RegisterStates() override
    {
        ClientLog::SG_SANDBOX_LOG_DEBUG("[Sandbox::RegisterStates()] Register State: NewState as Game.");
        GetStateStack().RegisterState<NewState>(sg::ogl::state::GAME);
    }

    void Init() override
    {
        ClientLog::SG_SANDBOX_LOG_DEBUG("[Sandbox::Init()] Init (Push) Game State.");
        GetStateStack().PushState(sg::ogl::state::GAME);
    }

private:

};

//-------------------------------------------------
// EntryPoint
//-------------------------------------------------

std::unique_ptr<sg::ogl::Application> sg::ogl::create_application()
{
    return std::make_unique<Sandbox>("res/config/Config.lua");
}
