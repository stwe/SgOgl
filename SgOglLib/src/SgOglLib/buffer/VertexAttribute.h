#pragma once

#include <string>
#include "SgOglException.h"

namespace sg::ogl::buffer
{
    enum class VertexAttributeType
    {
        NONE, POSITION, POSITION_2D, COLOR, UV, NORMAL
    };

    struct VertexAttribute
    {
        VertexAttributeType vertexAttributeType{ VertexAttributeType::NONE };
        std::string name;
        bool normalized{ false };
        uint32_t size{ 0 };
        uint64_t offset{ 0 };

        int32_t GetComponentCount() const
        {
            switch (vertexAttributeType)
            {
                case VertexAttributeType::POSITION: return 3;
                case VertexAttributeType::POSITION_2D: return 2;
                case VertexAttributeType::COLOR: return 3;
                case VertexAttributeType::UV: return 2;
                case VertexAttributeType::NORMAL: return 3;
                default:;
            }

            throw SG_OGL_EXCEPTION("[VertexAttribute::GetComponentCount()] Unknown VertexAttributeType.");
        }
    };
}
