#include "Mesh.h"
#include "Material.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Mesh::Mesh()
    : m_vao{ std::make_unique<buffer::Vao>() }
{
    SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Mesh()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[Mesh::Mesh()] Create Mesh.");
}

sg::ogl::resource::Mesh::~Mesh() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Mesh::Mesh()] Destruct Mesh.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::resource::Mesh::MaterialSharedPtr sg::ogl::resource::Mesh::GetDefaultMaterial() const
{
    SG_OGL_CORE_ASSERT(m_defaultMaterial, "[Mesh::GetDefaultMaterial()] Null pointer.")
    return m_defaultMaterial;
}

sg::ogl::resource::Mesh::VaoUniquePtr& sg::ogl::resource::Mesh::GetVao()
{
    return m_vao;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::resource::Mesh::SetDefaultMaterial(MaterialUniquePtr t_defaultMaterial)
{
    SG_OGL_CORE_ASSERT(!m_defaultMaterial, "[Mesh::SetDefaultMaterial()] Default material already exist.")
    m_defaultMaterial = std::move(t_defaultMaterial);
}

//-------------------------------------------------
// Draw mesh
//-------------------------------------------------

void sg::ogl::resource::Mesh::InitDraw() const
{
    m_vao->BindVao();
}

void sg::ogl::resource::Mesh::DrawPrimitives() const
{
    m_vao->DrawPrimitives();
}

void sg::ogl::resource::Mesh::DrawInstanced(const int32_t t_instanceCount) const
{
    m_vao->DrawInstanced(t_instanceCount);
}

void sg::ogl::resource::Mesh::EndDraw()
{
    buffer::Vao::UnbindVao();
}
