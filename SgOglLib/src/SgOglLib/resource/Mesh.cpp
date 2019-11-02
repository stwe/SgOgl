// This file is part of the SgOgl package.
// 
// Filename: Mesh.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Mesh.h"
#include "Core.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Mesh::Mesh()
    : m_vao{ std::make_unique<buffer::Vao>() }
{
    SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Mesh()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[Mesh::Mesh()] Create Mesh.");
}

sg::ogl::resource::Mesh::Mesh(const std::string& t_name)
    : m_vao{ std::make_unique<buffer::Vao>() }
    , m_name{ t_name }
{
    SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Mesh()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[Mesh::Mesh()] Create Mesh with name {}.", t_name);
}

sg::ogl::resource::Mesh::~Mesh() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Mesh::Mesh()] Destruct Mesh.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

std::string sg::ogl::resource::Mesh::GetName() const
{
    return m_name;
}

sg::ogl::resource::Mesh::MaterialSharedPtr sg::ogl::resource::Mesh::GetDefaultMaterial() const
{
    SG_OGL_CORE_ASSERT(m_defaultMaterial, "[Mesh::GetDefaultMaterial()] Null pointer.")
    return m_defaultMaterial;
}

sg::ogl::buffer::Vao& sg::ogl::resource::Mesh::GetVao() const
{
    return *m_vao;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::resource::Mesh::SetName(const std::string& t_name)
{
    m_name = t_name;
}

void sg::ogl::resource::Mesh::SetDefaultMaterial(const MaterialSharedPtr& t_defaultMaterial)
{
    SG_OGL_CORE_ASSERT(!m_defaultMaterial, "[Mesh::SetDefaultMaterial()] Default material already exist.")
    m_defaultMaterial = t_defaultMaterial;
}

//-------------------------------------------------
// Draw - methods created for convenience
//-------------------------------------------------

void sg::ogl::resource::Mesh::InitDraw() const
{
    m_vao->BindVao();
}

void sg::ogl::resource::Mesh::DrawPrimitives(const uint32_t t_drawMode) const
{
    m_vao->DrawPrimitives(t_drawMode);
}

void sg::ogl::resource::Mesh::DrawInstanced(const int32_t t_instanceCount, const uint32_t t_drawMode) const
{
    m_vao->DrawInstanced(t_instanceCount, t_drawMode);
}

void sg::ogl::resource::Mesh::EndDraw()
{
    buffer::Vao::UnbindVao();
}
