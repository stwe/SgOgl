#include "Mesh.h"
#include "Material.h"
#include "buffer/VertexAttribute.h"

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

sg::ogl::resource::Mesh::MaterialUniquePtr& sg::ogl::resource::Mesh::GetMaterial() noexcept
{
    return m_material; // todo if null
}

const sg::ogl::resource::Mesh::MaterialUniquePtr& sg::ogl::resource::Mesh::GetMaterial() const noexcept
{
    return m_material; // todo if null
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::resource::Mesh::SetMaterial(MaterialUniquePtr t_material)
{
    m_material = std::move(t_material);
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

void sg::ogl::resource::Mesh::EndDraw()
{
    buffer::Vao::UnbindVao();
}
