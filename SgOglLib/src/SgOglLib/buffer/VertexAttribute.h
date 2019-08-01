#pragma once

#include <string>
#include "Core.h"

namespace sg::ogl::buffer
{
    enum class VertexAttributeType
    {
        NONE, POSITION, POSITION_2D, COLOR, UV, NORMAL, TANGENT, BITANGENT
    };

    struct SG_OGL_LOCAL VertexAttribute
    {
        VertexAttributeType vertexAttributeType{ VertexAttributeType::NONE };
        std::string name;
        bool normalized{ false };
        uint32_t size{ 0 };
        uint64_t offset{ 0 };

        int32_t GetComponentCount() const;
    };
}
