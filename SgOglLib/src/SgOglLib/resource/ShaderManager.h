#pragma once

#include <string>
#include <memory>
#include <map>
#include "Core.h"
#include "Log.h"
#include "ShaderUtil.h"
#include "SgOglException.h"
#include "ShaderProgram.h"

namespace sg::ogl::resource
{
    class SG_OGL_API ShaderManager
    {
    public:
        using ShaderProgramUniquePtr = std::unique_ptr<ShaderProgram>;
        using ShaderProgramContainer = std::map<std::string, ShaderProgramUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderManager();

        ShaderManager(const ShaderManager& t_other) = delete;
        ShaderManager(ShaderManager&& t_other) noexcept = delete;
        ShaderManager& operator=(const ShaderManager& t_other) = delete;
        ShaderManager& operator=(ShaderManager&& t_other) noexcept = delete;

        ~ShaderManager() noexcept;

        //-------------------------------------------------
        // Add shader program
        //-------------------------------------------------

        template <typename T>
        void AddShaderProgram(const std::string& t_folder, const bool t_loadGeometryShader = false)
        {
            if (m_shaderPrograms.count(t_folder) != 0)
            {
                throw SG_OGL_EXCEPTION("[ShaderManager::AddShaderProgram()] Shader program " + t_folder + " already exist.");
            }

            auto shaderProgram{ std::make_unique<T>() };
            SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddShaderProgram()] Null pointer.")

            SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] Start adding shader to program: {}.", t_folder);

            const auto shaderPath{ "res/shader/" + t_folder };

            shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile(shaderPath + "/Vertex.vert"));
            shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile(shaderPath + "/Fragment.frag"));

            if (t_loadGeometryShader)
            {
                shaderProgram->AddGeometryShader(ShaderUtil::ReadShaderFile(shaderPath + "/Geometry.geom"));
            }

            shaderProgram->LinkAndValidateProgram();
            shaderProgram->AddAllFoundUniforms();

            m_shaderPrograms.emplace(t_folder, std::move(shaderProgram));

            SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] All shader was added successfully to program {}.", t_folder);
        }

        //void AddComputeShaderProgram(const std::string& t_fileName);

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ShaderProgram& GetShaderProgram(const std::string& t_name);
        const ShaderProgram& GetShaderProgram(const std::string& t_name) const;
        ShaderProgram& GetComputeShaderProgram(const std::string& t_name);
        const ShaderProgram& GetComputeShaderProgram(const std::string& t_name) const;

    protected:

    private:
        ShaderProgramContainer m_shaderPrograms;
        ShaderProgramContainer m_computeShaderPrograms;
    };
}
