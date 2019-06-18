#include "Vao.h"
#include "Log.h"
#include "OpenGl.h"
#include "BufferLayout.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vao::Vao()
{
    GenerateVao();
    BindVao();

    SG_OGL_CORE_LOG_DEBUG("[Vao::Vao()] A new Vao was created. Id: {}", m_vaoId);
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

auto sg::ogl::buffer::Vao::GetVaoId() const
{
    return m_vaoId;
}

const auto& sg::ogl::buffer::Vao::GetVbos() const
{
    return m_vbos;
}

auto sg::ogl::buffer::Vao::GetEboId() const
{
    return m_eboId;
}

auto sg::ogl::buffer::Vao::HasIndexBuffer() const
{
    return m_eboId != 0;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::buffer::Vao::SetDrawCount(const int32_t t_drawCount)
{
    m_drawCount = t_drawCount;
}

//-------------------------------------------------
// Vao - Public && Private
//-------------------------------------------------

void sg::ogl::buffer::Vao::GenerateVao()
{
    glGenVertexArrays(1, &m_vaoId);
}

void sg::ogl::buffer::Vao::BindVao() const
{
    SG_OGL_CORE_ASSERT(m_vaoId, "Invalid Vao-Id.")
    glBindVertexArray(m_vaoId);
}

void sg::ogl::buffer::Vao::UnbindVao()
{
    glBindVertexArray(0);
}

void sg::ogl::buffer::Vao::DeleteVao() const
{
    if (m_vaoId)
    {
        glDeleteVertexArrays(1, &m_vaoId);
        SG_OGL_CORE_LOG_DEBUG("[Vao::DeleteVao()] Vao was deleted. Id: {}", m_vaoId);
    }
}

//-------------------------------------------------
// Vbo`s
//-------------------------------------------------

auto sg::ogl::buffer::Vao::GenerateVbo()
{
    uint32_t vboId{ 0 };
    glGenBuffers(1, &vboId);
    SG_OGL_CORE_ASSERT(vboId, "Invalid Vbo-Id.")

    m_vbos.push_back(vboId);

    SG_OGL_CORE_LOG_DEBUG("[Vao::GenerateVbo()] A new Vbo was created. Id: {}", vboId);

    return vboId;
}

void sg::ogl::buffer::Vao::BindVbo(const uint32_t t_id) const
{
    if (std::find(m_vbos.begin(), m_vbos.end(), t_id) != m_vbos.end())
    {
        glBindBuffer(GL_ARRAY_BUFFER, t_id);
    }
    else
    {
        SG_OGL_CORE_LOG_ERROR("[Vao::BindVbo()] Can't find the Vbo Id. Id: {}", t_id);
    }
}

void sg::ogl::buffer::Vao::UnbindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sg::ogl::buffer::Vao::DeleteVbos() const
{
    for (const auto& vboId : m_vbos)
    {
        glDeleteBuffers(1, &vboId);
        SG_OGL_CORE_LOG_DEBUG("[Vao::DeleteVbos()] Vbo was deleted. Id: {}", vboId);
    }
}

//-------------------------------------------------
// Ebo
//-------------------------------------------------

void sg::ogl::buffer::Vao::GenerateEbo()
{
    SG_OGL_CORE_ASSERT(!m_eboId, "An index buffer already exists.")

    glGenBuffers(1, &m_eboId);
    SG_OGL_CORE_ASSERT(m_eboId, "Invalid Ebo-Id.")

    SG_OGL_CORE_LOG_DEBUG("[Vao::GenerateEbo()] A new Ebo was created. Id: {}", m_eboId);
}

void sg::ogl::buffer::Vao::BindEbo() const
{
    SG_OGL_CORE_ASSERT(m_eboId, "Invalid Ebo-Id.")
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
}

void sg::ogl::buffer::Vao::DeleteEbo() const
{
    if (m_eboId)
    {
        glDeleteBuffers(1, &m_eboId);
        SG_OGL_CORE_LOG_DEBUG("[Vao::DeleteEbo()] Ebo was deleted. Id: ", m_eboId);
    }
}

//-------------------------------------------------
// Allocate
//-------------------------------------------------

void sg::ogl::buffer::Vao::AllocateVertices(const sg::ogl::buffer::Vao::VerticesContainer& t_vertices)
{
    const auto elementSizeInBytes{ sizeof(Vertex) };
    const auto numberOfElements{ t_vertices.size() };

    BindVao();

    const auto vboId{ GenerateVbo() };
    BindVbo(vboId);

    // Specifies the target to which the buffer object is bound.
    const auto target{ GL_ARRAY_BUFFER };
    // Specifies the size in bytes of the buffer object's new data store.
    const auto size{ numberOfElements * elementSizeInBytes };
    // Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
    const auto data{ t_vertices.data() };
    // Specifies the expected usage pattern of the data store.
    const auto usage{ GL_STATIC_DRAW };

    // Create and initialize a buffer.
    glBufferData(target, size, data, usage);

    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE_IN_BYTES, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE_IN_BYTES, reinterpret_cast<void*>(SIZE3_F));
    glEnableVertexAttribArray(1);

    // uv attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE_IN_BYTES, reinterpret_cast<void*>(SIZE6_F));
    glEnableVertexAttribArray(2);

    UnbindVbo();
    UnbindVao();

    m_drawCount = numberOfElements;
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::Vao::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Vao::CleanUp()] Start the clean up process for Vao. Id: {}", m_vaoId);

    glDisableVertexAttribArray(0);

    UnbindVbo();
    DeleteVbos();

    DeleteEbo();

    UnbindVao();
    DeleteVao();
}
