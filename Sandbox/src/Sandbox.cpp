#include "SgOgl.h"

class Sandbox : public sg::ogl::Application
{
public:

protected:

private:

};

std::unique_ptr<sg::ogl::Application> sg::ogl::create_application()
{
    return std::make_unique<Sandbox>();
}
