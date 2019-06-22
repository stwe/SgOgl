#include "SgOgl.h"
#include "SgOglEntryPoint.h"
#include "PlaygroundState.h"

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
        SG_OGL_LOG_DEBUG("[Sandbox::Sandbox()] Execute the Sandbox constructor.");

        PushState(new PlaygroundState);

        // todo init shader im State

        m_shaderManager->AddShaderProgram("simple");
    }

    Sandbox(const Sandbox& t_other) = delete;
    Sandbox(Sandbox&& t_other) noexcept = delete;
    Sandbox& operator=(const Sandbox& t_other) = delete;
    Sandbox& operator=(Sandbox&& t_other) noexcept = delete;

    ~Sandbox() noexcept override
    {
        SG_OGL_LOG_DEBUG("[Sandbox::~Sandbox()] Execute the Sandbox destructor.");
    }

protected:

private:

};

//-------------------------------------------------
// EntryPoint
//-------------------------------------------------

std::unique_ptr<sg::ogl::Application> sg::ogl::create_application()
{
    #if defined(_WIN64) && defined(_MSC_VER)

        return std::make_unique<Sandbox>("res/config/Config.xml");

    #elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

        return std::make_unique<Sandbox>("/home/steffen/Dev/SgOgl/Sandbox/res/config/Config.xml");

    #else

        #error Unsupported platform or unsupported compiler!

    #endif
}
