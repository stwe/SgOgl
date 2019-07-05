#include "BufferLayout.h"
#include "VertexAttribute.h"
#include "OpenGl.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::BufferLayout::BufferLayout(const std::initializer_list<VertexAttribute>& t_vertexAttributes)
    : m_vertexAttributes{ t_vertexAttributes }
{
    CalculateOffsetsAndStride();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::buffer::BufferLayout::Attributes& sg::ogl::buffer::BufferLayout::GetAttributes() const noexcept
{
    return m_vertexAttributes;
}

int32_t sg::ogl::buffer::BufferLayout::GetStride() const
{
    return m_stride;
}

uint32_t sg::ogl::buffer::BufferLayout::GetFloatsPerLayout() const
{
    return m_floatsPerLayout;
}

uint32_t sg::ogl::buffer::BufferLayout::GetVertexAttributeTypeSize(const VertexAttributeType t_vertexAttributeType)
{
    static_assert(sizeof(float) == 4, "[BufferLayout::GetVertexAttributeTypeSize()] Float must be 4 bytes.");
    static constexpr auto SIZE_OF_FLOAT{ 4 };

    switch (t_vertexAttributeType)
    {
        case VertexAttributeType::POSITION: return SIZE_OF_FLOAT * 3;
        case VertexAttributeType::POSITION_2D: return SIZE_OF_FLOAT * 2;
        case VertexAttributeType::COLOR: return SIZE_OF_FLOAT * 3;
        case VertexAttributeType::UV: return SIZE_OF_FLOAT * 2;
        case VertexAttributeType::NORMAL: return SIZE_OF_FLOAT * 3;
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
        case VertexAttributeType::NORMAL: return GL_FLOAT;
        default:;
    }

    throw SG_OGL_EXCEPTION("[BufferLayout::GetOpenGlType()] Unknown VertexAttributeType.");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::buffer::BufferLayout::CalculateOffsetsAndStride()
{
    uint64_t offset{ 0 };
    m_stride = 0;

    for (auto& attribute : m_vertexAttributes)
    {
        attribute.size = GetVertexAttributeTypeSize(attribute.vertexAttributeType);

        attribute.offset = offset;
        offset += attribute.size;
        m_stride += attribute.size;

        m_floatsPerLayout += attribute.GetComponentCount();
    }
}
