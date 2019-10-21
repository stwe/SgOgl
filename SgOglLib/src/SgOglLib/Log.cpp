#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.h"

sg::ogl::Log::LoggerSharedPtr sg::ogl::Log::m_coreLogger;
sg::ogl::Log::LoggerSharedPtr sg::ogl::Log::m_clientLogger;

void sg::ogl::Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_coreLogger = spdlog::stdout_color_mt("SgOglLib");
#ifdef SG_OGL_DEBUG_BUILD
    m_coreLogger->set_level(spdlog::level::trace);
#else
    m_coreLogger->set_level(spdlog::level::info);
#endif

    m_clientLogger = spdlog::stdout_color_mt("Client");
#ifdef SG_OGL_DEBUG_BUILD
    m_clientLogger->set_level(spdlog::level::trace);
#else
    m_clientLogger->set_level(spdlog::level::info);
#endif
}

sg::ogl::Log::LoggerSharedPtr& sg::ogl::Log::GetCoreLogger()
{
    return m_coreLogger;
}

sg::ogl::Log::LoggerSharedPtr& sg::ogl::Log::GetClientLogger()
{
    return m_clientLogger;
}
