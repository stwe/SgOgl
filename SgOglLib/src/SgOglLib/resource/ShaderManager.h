#pragma once

#include <string>
#include <memory>
#include <map>
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderProgram;

    struct DeleteShaderProgram
    {
        void operator()(ShaderProgram* t_shaderProgram) const;
    };

    class SG_OGL_API ShaderManager
    {
    public:
        using ShaderProgramUniquePtr = std::unique_ptr<ShaderProgram, DeleteShaderProgram>;
        using ShaderPrograms = std::map<std::string, ShaderProgramUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderManager();

        ShaderManager(const ShaderManager& t_other) = delete;
        ShaderManager(ShaderManager&& t_other) noexcept = delete;
        ShaderManager& operator=(const ShaderManager& t_other) = delete;
        ShaderManager& operator=(ShaderManager&& t_other) noexcept = delete;

        ~ShaderManager() noexcept;;

        //-------------------------------------------------
        // Add shader program
        //-------------------------------------------------

        void AddShaderProgram(const std::string& t_folder, bool t_loadGeometryShader = false);
        void AddComputeShaderProgram(const std::string& t_folder, const std::string& t_fileName);

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ShaderPrograms& GetShaderPrograms() noexcept;
        const ShaderPrograms& GetShaderPrograms() const noexcept;

        ShaderProgramUniquePtr& GetShaderProgram(const std::string& t_name);

        ShaderPrograms& GetComputeShaderPrograms() noexcept;
        const ShaderPrograms& GetComputeShaderPrograms() const noexcept;

        ShaderProgramUniquePtr& GetComputeShaderProgram(const std::string& t_folder, const std::string& t_fileName);

    protected:

    private:
        ShaderPrograms m_shaderPrograms;
        ShaderPrograms m_computeShaderPrograms;
    };
}
