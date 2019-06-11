#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace sg::ogl
{
    class SG_OGL_API Log
    {
    public:
        using LoggerSharedPtr = std::shared_ptr<spdlog::logger>;

        static void Init();

        static LoggerSharedPtr& GetCoreLogger();
        static LoggerSharedPtr& GetClientLogger();

    protected:

    private:
        static LoggerSharedPtr m_coreLogger;
        static LoggerSharedPtr m_clientLogger;
    };
}

// Core log macros
#define SG_OGL_CORE_LOG_TRACE(...) ::sg::ogl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SG_OGL_CORE_LOG_DEBUG(...) ::sg::ogl::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define SG_OGL_CORE_LOG_INFO(...)  ::sg::ogl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SG_OGL_CORE_LOG_WARN(...)  ::sg::ogl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SG_OGL_CORE_LOG_ERROR(...) ::sg::ogl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SG_OGL_CORE_LOG_FATAL(...) ::sg::ogl::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SG_OGL_LOG_TRACE(...) ::sg::ogl::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SG_OGL_LOG_DEBUG(...) ::sg::ogl::Log::GetClientLogger()->debug(__VA_ARGS__)
#define SG_OGL_LOG_INFO(...)  ::sg::ogl::Log::GetClientLogger()->info(__VA_ARGS__)
#define SG_OGL_LOG_WARN(...)  ::sg::ogl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SG_OGL_LOG_ERROR(...) ::sg::ogl::Log::GetClientLogger()->error(__VA_ARGS__)
#define SG_OGL_LOG_FATAL(...) ::sg::ogl::Log::GetClientLogger()->critical(__VA_ARGS__)
