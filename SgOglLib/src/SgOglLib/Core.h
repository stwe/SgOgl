#pragma once

#if defined(_WIN64) && defined(_MSC_VER)
    #define SG_OGL_DEBUG_BREAK __debugbreak()
    #define DLL_IMPORT __declspec(dllimport)
    #define DLL_EXPORT __declspec(dllexport)
    #define DLL_LOCAL
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    #include <signal.h>
    #define SG_OGL_DEBUG_BREAK raise(SIGTRAP)
    #define DLL_IMPORT __attribute__ ((visibility ("default")))
    #define DLL_EXPORT __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
    #error Unsupported platform or unsupported compiler!
#endif

#ifdef SG_OGL_DLL
    #ifdef SG_OGL_DLL_EXPORTS
        #define SG_OGL_API DLL_EXPORT
    #else
        #define SG_OGL_API DLL_IMPORT
    #endif
    #define SG_OGL_LOCAL DLL_LOCAL
#else
    #define SG_OGL_API
    #define SG_OGL_LOCAL
#endif

#ifdef SG_OGL_DEBUG_BUILD
    #define SG_OGL_ENABLE_ASSERTS
#endif

#ifdef SG_OGL_ENABLE_ASSERTS
    #define SG_OGL_ASSERT(x, ...) { if(!(x)) { SG_OGL_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SG_OGL_DEBUG_BREAK; } }
    #define SG_OGL_CORE_ASSERT(x, ...) { if(!(x)) { SG_OGL_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SG_OGL_DEBUG_BREAK; } }
#else
    #define SG_OGL_ASSERT(x, ...)
    #define SG_OGL_CORE_ASSERT(x, ...)
#endif
