// This file is part of the SgOgl package.
// 
// Filename: SgOglException.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <sstream>
#include "SgOglException.h"

sg::ogl::SgOglException::SgOglException(const int t_line, const char* t_file, std::string t_message)
    : m_line{ t_line }
    , m_file{ t_file }
    , m_message{ std::move(t_message) }
{
}

const char* sg::ogl::SgOglException::what() const noexcept
{
    std::ostringstream oss;

    oss << "occurred: \n";
    oss << "[Type]: " << GetType() << "\n";
    oss << GetOriginString();

    m_whatBuffer = oss.str();

    return m_whatBuffer.c_str();
}

const char* sg::ogl::SgOglException::GetType() const noexcept
{
    return "SgOgl Exception";
}

int sg::ogl::SgOglException::GetLine() const noexcept
{
    return m_line;
}

const std::string& sg::ogl::SgOglException::GetFile() const noexcept
{
    return m_file;
}

std::string sg::ogl::SgOglException::GetOriginString() const noexcept
{
    std::ostringstream oss;

    oss << "[Message]: " << m_message << "\n";
    oss << "[File]: "    << m_file << "\n";
    oss << "[Line]: "    << m_line;

    return oss.str();
}
