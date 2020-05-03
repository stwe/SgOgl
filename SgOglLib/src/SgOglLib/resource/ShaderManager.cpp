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

sg::ogl::resource::ShaderManager::ShaderManager(const std::string& t_libResFolder)
    : m_libResFolder{ t_libResFolder }
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::ShaderManager()] Create ShaderManager.");
}

sg::ogl::resource::ShaderManager::~ShaderManager() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ShaderManager::~ShaderManager()] Destruct ShaderManager.");
}
