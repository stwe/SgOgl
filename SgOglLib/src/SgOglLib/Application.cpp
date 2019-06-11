#include "Application.h"
#include "Log.h"

void sg::ogl::Application::Run()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Run()] Application is started.");

    Init();

    while(true) {}
}

void sg::ogl::Application::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up Application...");
}

void sg::ogl::Application::Init()
{

}
