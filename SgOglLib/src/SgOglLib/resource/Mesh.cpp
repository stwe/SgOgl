#include "Mesh.h"

sg::ogl::resource::Mesh::Mesh()
{
    m_vao = std::make_unique<buffer::Vao>();
    SG_OGL_CORE_ASSERT(m_vao, "[Mesh::Mesh()] Null pointer.")
}

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
