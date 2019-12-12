// This file is part of the SgOgl package.
// 
// Filename: BufferLayout.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "BufferLayout.h"
#include "OpenGl.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::BufferLayout::BufferLayout(const std::initializer_list<VertexAttribute>& t_vertexAttributes)
    : m_vertexAttributes{ t_vertexAttributes }
{
    Init();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::buffer::BufferLayout::AttributeContainer& sg::ogl::buffer::BufferLayout::GetAttributes() const noexcept
{
    return m_vertexAttributes;
}

int32_t sg::ogl::buffer::BufferLayout::GetStride() const
{
    return m_stride;
}

// todo: The name does not make sense if the layout contains other data types as floats.
uint32_t sg::ogl::buffer::BufferLayout::GetNumberOfFloats() const
{
    return m_numberOfFloats;
}

uint32_t sg::ogl::buffer::BufferLayout::GetVertexAttributeTypeSize(const VertexAttributeType t_vertexAttributeType)
{
    static_assert(sizeof(float) == 4, "[BufferLayout::GetVertexAttributeTypeSize()] Float must be 4 bytes.");

    switch (t_vertexAttributeType)
    {
        case VertexAttributeType::POSITION: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::POSITION);
        case VertexAttributeType::POSITION_2D: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::POSITION_2D);
        case VertexAttributeType::COLOR: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::COLOR);
        case VertexAttributeType::UV: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::UV);
        case VertexAttributeType::NORMAL: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::NORMAL);
        case VertexAttributeType::TANGENT: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::TANGENT);
        case VertexAttributeType::BITANGENT: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::BITANGENT);
        case VertexAttributeType::BONE_IDS: return sizeof(int32_t) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::BONE_IDS);
        case VertexAttributeType::WEIGHTS: return sizeof(float) * VertexAttributeMeta::GetComponentCount(VertexAttributeType::WEIGHTS);
        default:;
    }

    throw SG_OGL_EXCEPTION("[BufferLayout::GetVertexAttributeTypeSize()] Unknown VertexAttributeType.");
}

uint32_t sg::ogl::buffer::BufferLayout::GetOpenGlType(const VertexAttributeType t_vertexAttributeType)
{
    switch (t_vertexAttributeType)
    {
        case VertexAttributeType::POSITION:
        case VertexAttributeType::POSITION_2D:
        case VertexAttributeType::COLOR:
        case VertexAttributeType::UV:
        case VertexAttributeType::NORMAL:
        case VertexAttributeType::TANGENT:
        case VertexAttributeType::BITANGENT: return GL_FLOAT;
        case VertexAttributeType::BONE_IDS: return GL_INT;
        case VertexAttributeType::WEIGHTS: return GL_FLOAT;
        default:;
    }

    throw SG_OGL_EXCEPTION("[BufferLayout::GetOpenGlType()] Unknown VertexAttributeType.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::buffer::BufferLayout::Init()
{
    uint64_t offset{ 0 };

    for (auto& attribute : m_vertexAttributes)
    {
        attribute.size = GetVertexAttributeTypeSize(attribute.vertexAttributeType);

        attribute.offset = offset;
        offset += attribute.size;

        m_stride += attribute.size;

        m_numberOfFloats += VertexAttributeMeta::GetComponentCount(attribute.vertexAttributeType);
    }
}
