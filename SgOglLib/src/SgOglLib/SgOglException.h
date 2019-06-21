#pragma once

#include <exception>
#include <string>

namespace sg::ogl
{
    class SgOglException : public std::exception
    {
    public:
        SgOglException(int t_line, const char* t_file, std::string t_message);

        char const* what() const override;

        virtual const char* GetType() const;

        int GetLine() const;
        const std::string& GetFile() const noexcept;
        std::string GetOriginString() const;

    protected:
        mutable std::string m_whatBuffer;

    private:
        int m_line;
        std::string m_file;
        std::string m_message;
    };
}

#define SG_OGL_EXCEPTION(message) sg::ogl::SgOglException(__LINE__, __FILE__, message)
