#include "VertexAttribute.h"
#include "SgOglException.h"

int32_t sg::ogl::buffer::VertexAttribute::GetComponentCount() const
{
    switch (vertexAttributeType)
    {
        case VertexAttributeType::POSITION: return 3;
        case VertexAttributeType::POSITION_2D: return 2;
        case VertexAttributeType::COLOR: return 3;
        case VertexAttributeType::UV: return 2;
        case VertexAttributeType::NORMAL: return 3;
        case VertexAttributeType::TANGENT: return 3;
        case VertexAttributeType::BITANGENT: return 3;
        default:;
    }

    throw SG_OGL_EXCEPTION("[VertexAttribute::GetComponentCount()] Unknown VertexAttributeType.");
}
