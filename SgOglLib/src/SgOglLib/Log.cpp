#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

sg::ogl::Log::LoggerSharedPtr sg::ogl::Log::m_coreLogger;
sg::ogl::Log::LoggerSharedPtr sg::ogl::Log::m_clientLogger;

void sg::ogl::Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_coreLogger = spdlog::stdout_color_mt("SgOglLib");
    m_coreLogger->set_level(spdlog::level::trace);

    m_clientLogger = spdlog::stdout_color_mt("Application");
    m_clientLogger->set_level(spdlog::level::trace);
}

sg::ogl::Log::LoggerSharedPtr& sg::ogl::Log::GetCoreLogger()
{
    return m_coreLogger;
}

sg::ogl::Log::LoggerSharedPtr& sg::ogl::Log::GetClientLogger()
{
    return m_clientLogger;
}
