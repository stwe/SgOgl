#include "SgException.h"

sg::ogl::SgException::SgException(const std::string& t_message)
    : std::runtime_error{ t_message }
    , m_msg{ t_message }
{
}

const char* sg::ogl::SgException::what() const noexcept
{
    return m_msg.c_str();
}
