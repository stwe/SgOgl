#include "Vao.h"
#include "Log.h"
#include "OpenGl.h"
#include "BufferLayout.h"
#include "VertexAttribute.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vao::Vao()
{
    GenerateVao();
    BindVao();

    SG_OGL_CORE_LOG_DEBUG("[Vao::Vao()] A new Vao was created. Id: {}", m_vaoId);
}

sg::ogl::buffer::Vao::~Vao() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Vao::~Vao()] Destruct Vao.");
    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::buffer::Vao::GetVaoId() const
{
    return m_vaoId;
}

const sg::ogl::buffer::Vao::Vbos& sg::ogl::buffer::Vao::GetVbos() const
{
    return m_vbos;
}

uint32_t sg::ogl::buffer::Vao::GetEboId() const
{
    return m_eboId;
}

bool sg::ogl::buffer::Vao::HasIndexBuffer() const
{
    return m_eboId != 0;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::buffer::Vao::SetDrawCount(const int32_t t_drawCount)
{
    SG_OGL_CORE_ASSERT(t_drawCount > 0, "[Vao::SetDrawCount()] Invalid value.")
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
    SG_OGL_CORE_ASSERT(m_vaoId, "[Vao::BindVao()] Invalid Vao Id.")
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

uint32_t sg::ogl::buffer::Vao::GenerateVbo()
{
    uint32_t vboId{ 0 };
    glGenBuffers(1, &vboId);
    SG_OGL_CORE_ASSERT(vboId, "[Vao::GenerateVbo()] Error while creating a new Vbo.")

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
        throw SG_OGL_EXCEPTION("[Vao::BindVbo()] Can't find the Vbo Id. Id: " + std::to_string(t_id));
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
    SG_OGL_CORE_ASSERT(!m_eboId, "[Vao::GenerateEbo()] An Ebo already exists.")

    glGenBuffers(1, &m_eboId);
    SG_OGL_CORE_ASSERT(m_eboId, "[Vao::GenerateEbo()] Error while creating a new Ebo.")

    SG_OGL_CORE_LOG_DEBUG("[Vao::GenerateEbo()] A new Ebo was created. Id: {}", m_eboId);
}

void sg::ogl::buffer::Vao::BindEbo() const
{
    SG_OGL_CORE_ASSERT(m_eboId, "[Vao::BindEbo()] Invalid Ebo Id.")
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
}

void sg::ogl::buffer::Vao::DeleteEbo() const
{
    if (m_eboId)
    {
        glDeleteBuffers(1, &m_eboId);
        SG_OGL_CORE_LOG_DEBUG("[Vao::DeleteEbo()] Ebo was deleted. Id: {}", m_eboId);
    }
}

//-------------------------------------------------
// Allocate
//-------------------------------------------------

void sg::ogl::buffer::Vao::AllocateIndices(const IndicesContainer& t_indices)
{
    static constexpr auto ELEMENT_SIZE_IN_BYTES{ sizeof(uint32_t) };
    const auto numberOfElements{ static_cast<int32_t>(t_indices.size()) };

    // Bind our existing Vao.
    BindVao();

    // Generate and bind a new Ebo.
    GenerateEbo();
    BindEbo();

    // Create and initialize a buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfElements * ELEMENT_SIZE_IN_BYTES, t_indices.data(), GL_STATIC_DRAW);

    // Unbind Vao.
    UnbindVao();

    // Set draw count.
    SetDrawCount(numberOfElements);
}

void sg::ogl::buffer::Vao::AllocateVertices(float* const t_vertices, const int32_t t_drawCount, const uint32_t t_size, const BufferLayout& t_bufferLayout)
{
    // Bind our existing Vao.
    BindVao();

    // Generate and bind a new Vbo.
    const auto vboId{ GenerateVbo() };
    BindVbo(vboId);

    // Specifies the target to which the buffer object is bound.
    const auto target{ GL_ARRAY_BUFFER };

    // Specifies the expected usage pattern of the data store.
    const auto usage{ GL_STATIC_DRAW };

    // Create and initialize a buffer.
    glBufferData(target, t_size, t_vertices, usage);

    // Specify how OpenGL should interpret the vertex data before rendering.
    uint32_t index{ 0 };
    for (const auto& attribute : t_bufferLayout.GetAttributes())
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            attribute.GetComponentCount(),
            BufferLayout::GetOpenGlType(attribute.vertexAttributeType),
            attribute.normalized ? GL_TRUE : GL_FALSE,
            t_bufferLayout.GetStride(),
            reinterpret_cast<uintptr_t*>(attribute.offset)
        );

        index++;
    }

    // Unbind buffers.
    UnbindVbo();
    UnbindVao();

    // Set draw count.
    SetDrawCount(t_drawCount);
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives() const
{
    if (HasIndexBuffer())
    {
        glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::Vao::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Vao::CleanUp()] Start the OpenGL clean up process for Vao. Id: {}", m_vaoId);

    glDisableVertexAttribArray(0);

    UnbindVbo();
    DeleteVbos();

    DeleteEbo();

    UnbindVao();
    DeleteVao();
}
