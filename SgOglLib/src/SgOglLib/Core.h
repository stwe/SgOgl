#pragma once

#ifndef _WIN64
    #error Unsupported platform.
#endif

#ifdef SG_OGL_DEBUG_BUILD
    #define SG_OGL_ENABLE_ASSERTS
#endif

#ifdef SG_OGL_ENABLE_ASSERTS
    #define SG_OGL_ASSERT(x, ...) { if(!(x)) { SG_OGL_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define SG_OGL_CORE_ASSERT(x, ...) { if(!(x)) { SG_OGL_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define SG_OGL_ASSERT(x, ...)
    #define SG_OGL_CORE_ASSERT(x, ...)
#endif
