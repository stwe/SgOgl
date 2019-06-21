#pragma once

#include "SgOglException.h"
#include "Log.h"

int main()
{
    sg::ogl::Log::Init();

    SG_OGL_CORE_LOG_DEBUG("[main()] Starting main.");
    SG_OGL_CORE_LOG_DEBUG("[main()] Logger was initialized.");

    try
    {
        auto appUniquePtr{ sg::ogl::create_application() };
        appUniquePtr->Run();
        return EXIT_SUCCESS;
    }
    catch (const sg::ogl::SgOglException& e)
    {
        SG_OGL_CORE_LOG_ERROR("SgOglException {}", e.what());
    }
    catch (const std::exception& e)
    {
        SG_OGL_CORE_LOG_ERROR("Standard Exception: {}", e.what());
    }
    catch ( ... )
    {
        SG_OGL_CORE_LOG_ERROR("Unknown Exception. No details available.");
    }

    return EXIT_FAILURE;
}
