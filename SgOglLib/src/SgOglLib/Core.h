// This file is part of the SgOgl package.
// 
// Filename: Core.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
    #define SG_OGL_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    #include <signal.h>
    #define SG_OGL_DEBUG_BREAK raise(SIGTRAP)
#else
    #error Unsupported platform or compiler!
#endif

#ifdef SG_OGL_DEBUG_BUILD
    #define SG_OGL_ENABLE_ASSERTS
#endif

#ifdef SG_OGL_ENABLE_ASSERTS

    inline auto SG_OGL_CORE_ASSERT = [](auto&& t_expr, std::string_view t_str) -> void
    {
        if (!(t_expr))
        {
            sg::ogl::Log::SG_OGL_CORE_LOG_ERROR("Assertion Failed: {}", t_str);
            SG_OGL_DEBUG_BREAK;
        }
    };

#else

    inline auto SG_OGL_CORE_ASSERT = [](auto&& t_expr, std::string_view t_str) -> void
    {};

#endif
