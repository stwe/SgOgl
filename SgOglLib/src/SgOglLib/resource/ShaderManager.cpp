// This file is part of the SgOgl package.
// 
// Filename: ShaderManager.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "ShaderManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ShaderManager::ShaderManager()
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::ShaderManager()] Create ShaderManager.");
}

sg::ogl::resource::ShaderManager::~ShaderManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::~ShaderManager()] Destruct ShaderManager.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::resource::ShaderProgram& sg::ogl::resource::ShaderManager::GetShaderProgram(const std::string& t_name)
{
    if (m_shaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetShaderProgram()] Shader program " + t_name + " not exist.");
    }

    return *m_shaderPrograms.at(t_name);
}

const sg::ogl::resource::ShaderProgram& sg::ogl::resource::ShaderManager::GetShaderProgram(const std::string& t_name) const
{
    if (m_shaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetShaderProgram()] Shader program " + t_name + " not exist.");
    }

    return *m_shaderPrograms.at(t_name);
}

sg::ogl::resource::ShaderProgram& sg::ogl::resource::ShaderManager::GetComputeShaderProgram(const std::string& t_name)
{
    if (m_computeShaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program " + t_name + " not exist.");
    }

    return *m_computeShaderPrograms.at(t_name);
}

const sg::ogl::resource::ShaderProgram& sg::ogl::resource::ShaderManager::GetComputeShaderProgram(const std::string& t_name) const
{
    if (m_computeShaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program " + t_name + " not exist.");
    }

    return *m_computeShaderPrograms.at(t_name);
}
