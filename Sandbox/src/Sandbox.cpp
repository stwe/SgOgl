#include "SgOgl.h"
#include "SgOglEntryPoint.h"
#include "GameState.h"

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
        SG_OGL_LOG_DEBUG("[Sandbox::RegisterStates()] Register Game State.");
        GetStateStack()->RegisterState<GameState>(sg::ogl::state::GAME);
    }

    void Init() override
    {
        SG_OGL_LOG_DEBUG("[Sandbox::Init()] Init (Push) Game State.");
        GetStateStack()->PushState(sg::ogl::state::GAME);
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
