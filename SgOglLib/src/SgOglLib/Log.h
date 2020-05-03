// This file is part of the SgOgl package.
// 
// Filename: Log.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sg::ogl
{
    class Log
    {
    public:
        using LoggerSharedPtr = std::shared_ptr<spdlog::logger>;

        static void Init()
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");

            m_libLogger = spdlog::stdout_color_mt("SgOglLib");

#ifdef SG_OGL_DEBUG_BUILD
            m_libLogger->set_level(spdlog::level::trace);
#else
            m_libLogger->set_level(spdlog::level::info);
#endif
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_TRACE(T ...t_args)
        {
            m_libLogger->trace(t_args...);
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_DEBUG(T ...t_args)
        {
            m_libLogger->debug(t_args...);
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_INFO(T ...t_args)
        {
            m_libLogger->info(t_args...);
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_WARN(T ...t_args)
        {
            m_libLogger->warn(t_args...);
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_ERROR(T ...t_args)
        {
            m_libLogger->error(t_args...);
        }

        template <typename ...T>
        static void SG_OGL_CORE_LOG_FATAL(T ...t_args)
        {
            m_libLogger->critical(t_args...);
        }

    protected:

    private:
        inline static LoggerSharedPtr m_libLogger;
    };
}
