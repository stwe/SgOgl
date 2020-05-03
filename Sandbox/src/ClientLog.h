// This file is part of the SgOgl package.
// 
// Filename: ClientLog.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class ClientLog
{
public:
    using LoggerSharedPtr = std::shared_ptr<spdlog::logger>;

    static void Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_logger = spdlog::stdout_color_mt("Sandbox");

#ifdef SG_OGL_DEBUG_BUILD
        m_logger->set_level(spdlog::level::trace);
#else
        m_logger->set_level(spdlog::level::info);
#endif
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_TRACE(T ...t_args)
    {
        m_logger->trace(t_args...);
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_DEBUG(T ...t_args)
    {
        m_logger->debug(t_args...);
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_INFO(T ...t_args)
    {
        m_logger->info(t_args...);
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_WARN(T ...t_args)
    {
        m_logger->warn(t_args...);
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_ERROR(T ...t_args)
    {
        m_logger->error(t_args...);
    }

    template <typename ...T>
    static void SG_SANDBOX_LOG_FATAL(T ...t_args)
    {
        m_logger->critical(t_args...);
    }

protected:

private:
    inline static LoggerSharedPtr m_logger;
};

