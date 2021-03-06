// This file is part of the SgOgl package.
// 
// Filename: ShaderManager.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include <memory>
#include <map>
#include <typeindex>
#include "ShaderUtil.h"
#include "SgOglException.h"
#include "ShaderProgram.h"
#include "Core.h"

namespace sg::ogl::resource
{
    class ShaderManager
    {
    public:
        using ShaderProgramUniquePtr = std::unique_ptr<ShaderProgram>;
        using ShaderProgramContainer = std::map<std::type_index, ShaderProgramUniquePtr>;
        using ComputeShaderProgramContainer = std::map<std::type_index, ShaderProgramUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderManager() = delete;

        explicit ShaderManager(const std::string& t_libResFolder);

        ShaderManager(const ShaderManager& t_other) = delete;
        ShaderManager(ShaderManager&& t_other) noexcept = delete;
        ShaderManager& operator=(const ShaderManager& t_other) = delete;
        ShaderManager& operator=(ShaderManager&& t_other) noexcept = delete;

        ~ShaderManager() noexcept;

        //-------------------------------------------------
        // Add shader program
        //-------------------------------------------------

        template <typename T>
        void AddShaderProgram()
        {
            // just add the shader program to the manager if it does not already exist
            if (m_shaderPrograms.count(typeid(T)) == 0)
            {
                // create ShaderProgram
                auto shaderProgram{ std::make_unique<T>() };
                SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddShaderProgram()] Null pointer.");

                // get folder name
                auto folderName{ shaderProgram->GetFolderName() };

                // create path
                std::string shaderPath;
                if (shaderProgram->IsBuiltIn())
                {
                    shaderPath = m_libResFolder + "/shader/" + folderName;
                }
                else
                {
                    shaderPath = "res/shader/" + folderName;
                }

                // get options
                const auto options{ shaderProgram->GetOptions() };

                Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] Start adding shader to program: {}.", folderName);

                // add vertex shader
                if ((options & ShaderProgram::Flags::VERTEX_SHADER) == ShaderProgram::Flags::VERTEX_SHADER)
                {
                    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile(shaderPath + "/Vertex.vert"));
                }

                // add tessellation control shader
                if ((options & ShaderProgram::Flags::TESSELLATION_CONTROL_SHADER) == ShaderProgram::Flags::TESSELLATION_CONTROL_SHADER)
                {
                    shaderProgram->AddTessellationControlShader(ShaderUtil::ReadShaderFile(shaderPath + "/TessControl.tesc"));
                }

                // add tessellation evaluation shader
                if ((options & ShaderProgram::Flags::TESSELLATION_EVALUATION_SHADER) == ShaderProgram::Flags::TESSELLATION_EVALUATION_SHADER)
                {
                    shaderProgram->AddTessellationEvaluationShader(ShaderUtil::ReadShaderFile(shaderPath + "/TessEval.tese"));
                }

                // add geometry shader
                if ((options & ShaderProgram::Flags::GEOMETRY_SHADER) == ShaderProgram::Flags::GEOMETRY_SHADER)
                {
                    shaderProgram->AddGeometryShader(ShaderUtil::ReadShaderFile(shaderPath + "/Geometry.geom"));
                }

                // add fragment shader
                if ((options & ShaderProgram::Flags::FRAGMENT_SHADER) == ShaderProgram::Flags::FRAGMENT_SHADER)
                {
                    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile(shaderPath + "/Fragment.frag"));
                }

                shaderProgram->LinkAndValidateProgram();
                shaderProgram->AddAllFoundUniforms();

                m_shaderPrograms.emplace(typeid(T), std::move(shaderProgram));

                Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] All shader was added successfully to program {}.", folderName);
            }
        }

        template <typename T>
        void AddComputeShaderProgram()
        {
            // just add the compute shader program to the manager if it does not already exist
            if (m_computeShaderPrograms.count(typeid(T)) == 0)
            {
                auto shaderProgram{ std::make_unique<T>() };
                SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddComputeShaderProgram()] Null pointer.");

                // get file name
                auto fileName{ shaderProgram->GetFolderName() + ".comp" };

                // create full path
                std::string shader;
                if (shaderProgram->IsBuiltIn())
                {
                    shader = m_libResFolder + "/shader/compute/" + fileName;
                }
                else
                {
                    shader = "res/shader/compute/" + fileName;
                }

                Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddComputeShaderProgram()] Start adding compute shader to program: {}.", fileName);

                shaderProgram->AddComputeShader(ShaderUtil::ReadShaderFile(shader));
                shaderProgram->LinkAndValidateProgram();
                shaderProgram->AddAllFoundUniforms();
;
                m_computeShaderPrograms.emplace(typeid(T), std::move(shaderProgram));

                Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddComputeShaderProgram()] A compute shader was added successfully to program {}.", fileName);
            }
        }

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        template <typename T>
        [[nodiscard]] ShaderProgram& GetShaderProgram()
        {
            if (m_shaderPrograms.count(typeid(T)) == 0)
            {
                throw SG_OGL_EXCEPTION("[ShaderManager::GetShaderProgram()] Shader program not exist.");
            }

            return *m_shaderPrograms.at(typeid(T));
        }

        template <typename T>
        [[nodiscard]] const ShaderProgram& GetShaderProgram() const
        {
            if (m_shaderPrograms.count(typeid(T)) == 0)
            {
                throw SG_OGL_EXCEPTION("[ShaderManager::GetShaderProgram()] Shader program not exist.");
            }

            return *m_shaderPrograms.at(typeid(T));
        }

        template <typename T>
        [[nodiscard]] ShaderProgram& GetComputeShaderProgram()
        {
            if (m_computeShaderPrograms.count(typeid(T)) == 0)
            {
                throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program not exist.");
            }

            return *m_computeShaderPrograms.at(typeid(T));
        }

        template <typename T>
        [[nodiscard]] const ShaderProgram& GetComputeShaderProgram() const
        {
            if (m_computeShaderPrograms.count(typeid(T)) == 0)
            {
                throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program not exist.");
            }

            return *m_computeShaderPrograms.at(typeid(T));
        }

    protected:

    private:
        std::string m_libResFolder;

        ShaderProgramContainer m_shaderPrograms;
        ComputeShaderProgramContainer m_computeShaderPrograms;
    };
}
