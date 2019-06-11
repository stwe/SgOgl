#pragma once

int main()
{
    sg::ogl::Log::Init();

    SG_OGL_CORE_LOG_DEBUG("[main()] Starting main.");
    SG_OGL_CORE_LOG_DEBUG("[main()] Logger initialized.");

    try
    {
        auto appUniquePtr{ sg::ogl::create_application() };
        appUniquePtr->Run();
    }
    catch (const std::exception& e)
    {
        SG_OGL_CORE_LOG_ERROR("Exception: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
