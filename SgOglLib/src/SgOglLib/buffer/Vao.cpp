// This file is part of the SgOgl package.
// 
// Filename: Vao.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Vao.h"
#include "Vbo.h"
#include "Log.h"
#include "Core.h"
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

const sg::ogl::buffer::Vao::VboContainer& sg::ogl::buffer::Vao::GetVbos() const
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
// Vao
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
// Add buffer
//-------------------------------------------------

void sg::ogl::buffer::Vao::AddVertexDataVbo(float* const t_vertices, const int32_t t_drawCount, const BufferLayout& t_bufferLayout)
{
    // Bind our existing Vao.
    BindVao();

    // Generate a new Vbo.
    const auto vboId{ Vbo::GenerateVbo() };

    // Store the Vbo Id.
    m_vbos.push_back(vboId);

    // Bind the new Vbo.
    Vbo::BindVbo(vboId);

    // Calc the number of floats.
    const auto floatCount{ t_bufferLayout.GetNumberOfFloats() * t_drawCount };

    // Create and initialize a buffer.
    glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), t_vertices, GL_STATIC_DRAW);

    // Specify how OpenGL should interpret the vertex data before rendering.
    uint32_t index{ 0 };
    for (const auto& attribute : t_bufferLayout.GetAttributes())
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            VertexAttributeMeta::GetComponentCount(attribute.vertexAttributeType),
            BufferLayout::GetOpenGlType(attribute.vertexAttributeType),
            attribute.normalized ? GL_TRUE : GL_FALSE,
            t_bufferLayout.GetStride(),
            reinterpret_cast<uintptr_t*>(attribute.offset)
        );

        index++;
    }

    // Unbind buffers.
    Vbo::UnbindVbo();
    UnbindVao();

    // Set draw count.
    SetDrawCount(t_drawCount);
}

void sg::ogl::buffer::Vao::AddIndexBuffer(const IndexContainer& t_indices)
{
    static constexpr auto ELEMENT_SIZE_IN_BYTES{ sizeof(uint32_t) };
    const auto numberOfElements{ static_cast<int32_t>(t_indices.size()) };

    if (m_vbos.empty())
    {
        SG_OGL_CORE_LOG_WARN("[Vao::AddIndexBuffer()] The index buffer should be created last, otherwise wrong values in draw count are likely.");
    }

    // Bind our existing Vao.
    BindVao();

    // Generate and bind a new Ebo.
    SG_OGL_CORE_ASSERT(!m_eboId, "[Vao::AddIndexBuffer()] The Ebo already exists.")
    m_eboId = Vbo::GenerateEbo();
    Vbo::BindEbo(m_eboId);

    // Create and initialize a buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfElements * ELEMENT_SIZE_IN_BYTES, t_indices.data(), GL_STATIC_DRAW);

    // Unbind Vao.
    UnbindVao();

    // Set draw count.
    SetDrawCount(numberOfElements);
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode) const
{
    if (HasIndexBuffer())
    {
        glDrawElements(t_drawMode, m_drawCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(t_drawMode, 0, m_drawCount);
    }
}

void sg::ogl::buffer::Vao::DrawInstanced(const int32_t t_instanceCount, const uint32_t t_drawMode) const
{
    if (HasIndexBuffer())
    {
        glDrawElementsInstanced(t_drawMode, m_drawCount, GL_UNSIGNED_INT, nullptr, t_instanceCount);
    }
    else
    {
        glDrawArraysInstanced(t_drawMode, 0, m_drawCount, t_instanceCount);
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::Vao::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[Vao::CleanUp()] Start the OpenGL clean up process for Vao. Id: {}", m_vaoId);

    glDisableVertexAttribArray(0);

    // delete Vbos
    Vbo::UnbindVbo();
    for (const auto& vboId : m_vbos)
    {
        Vbo::DeleteVbo(vboId);
    }

    // delete Vbo / IndexBuffer
    if (m_eboId)
    {
        Vbo::DeleteEbo(m_eboId);
    }

    // delete Vao
    UnbindVao();
    DeleteVao();
}
