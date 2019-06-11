#pragma once

#include <sstream>
#include "Log.h"

#define SG_OGL_EXCEPTION(message) sg_ogl_exception(message, __FILE__, __LINE__)

namespace sg::ogl
{
    class SgException : public std::runtime_error
    {
    public:
        explicit SgException(const std::string& t_message);

        SgException(const SgException& t_other) = delete;
        SgException(SgException&& t_other) noexcept = delete;
        SgException& operator=(const SgException& t_other) = delete;
        SgException& operator=(SgException&& t_other) noexcept = delete;

        ~SgException() noexcept override = default;

        const char* what() const noexcept override;

    protected:

    private:
        std::string m_msg;
    };

    inline void sg_ogl_exception(const std::string& t_message, const char* t_fileName, const std::size_t t_lineNumber)
    {
        std::ostringstream stream;
        stream << "SG_OGL_EXCEPTION: " << t_message << " File: " << t_fileName << " Line: " << t_lineNumber;

        SG_OGL_CORE_LOG_ERROR("SG_OGL_EXCEPTION: {}", stream.str());

        throw SgException(stream.str());
    }
}
