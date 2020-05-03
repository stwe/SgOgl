// This file is part of the SgOgl package.
// 
// Filename: EntryPoint.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOglException.h"
#include "Log.h"

int main()
{
    sg::ogl::Log::Init();

    sg::ogl::Log::SG_OGL_CORE_LOG_DEBUG("[main()] Starting main.");
    sg::ogl::Log::SG_OGL_CORE_LOG_DEBUG("[main()] Logger was initialized.");

    try
    {
        auto appUniquePtr{ sg::ogl::create_application() };
        appUniquePtr->Run();
        return EXIT_SUCCESS;
    }
    catch (const sg::ogl::SgOglException& e)
    {
        sg::ogl::Log::SG_OGL_CORE_LOG_ERROR("SgOglException {}", e.what());
    }
    catch (const std::exception& e)
    {
        sg::ogl::Log::SG_OGL_CORE_LOG_ERROR("Standard Exception: {}", e.what());
    }
    catch ( ... )
    {
        sg::ogl::Log::SG_OGL_CORE_LOG_ERROR("Unknown Exception. No details available.");
    }

    return EXIT_FAILURE;
}
