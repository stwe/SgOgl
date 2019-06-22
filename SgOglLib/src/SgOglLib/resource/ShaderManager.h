#pragma once

#include <string>
#include <memory>
#include <map>
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderProgram;

    class SG_OGL_API ShaderManager
    {
    public:
        using ShaderProgramUniquePtr = std::unique_ptr<ShaderProgram>;
        using ShaderPrograms = std::map<std::string, ShaderProgramUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderManager() = default;

        ShaderManager(const ShaderManager& t_other) = delete;
        ShaderManager(ShaderManager&& t_other) noexcept = delete;
        ShaderManager& operator=(const ShaderManager& t_other) = delete;
        ShaderManager& operator=(ShaderManager&& t_other) noexcept = delete;

        ~ShaderManager() noexcept = default;

        //-------------------------------------------------
        // Add shader program
        //-------------------------------------------------

        void AddShaderProgram(const std::string& t_folder);

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ShaderPrograms& GetShaderPrograms() noexcept;
        const ShaderPrograms& GetShaderPrograms() const noexcept;

        ShaderProgramUniquePtr& GetShaderProgram(const std::string& t_name);

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp();

    protected:

    private:
        ShaderPrograms m_shaderPrograms;
    };
}
