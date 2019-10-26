// This file is part of the SgOgl package.
// 
// Filename: VertexAttribute.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include "Core.h"
#include "SgOglException.h"

namespace sg::ogl::buffer
{
    enum class VertexAttributeType
    {
        NONE, POSITION, POSITION_2D, COLOR, UV, NORMAL, TANGENT, BITANGENT
    };

    struct VertexAttributeMeta
    {
        static int32_t GetComponentCount(const VertexAttributeType t_vertexAttributeType)
        {
            switch (t_vertexAttributeType)
            {
                case VertexAttributeType::POSITION: return 3;
                case VertexAttributeType::POSITION_2D: return 2;
                case VertexAttributeType::COLOR: return 3;
                case VertexAttributeType::UV: return 2;
                case VertexAttributeType::NORMAL: return 3;
                case VertexAttributeType::TANGENT: return 3;
                case VertexAttributeType::BITANGENT: return 3;
                default:;

                throw SG_OGL_EXCEPTION("[VertexAttributeMeta::GetComponentCount()] Unknown VertexAttributeType.");
            }
        }
    };

    struct SG_OGL_LOCAL VertexAttribute
    {
        VertexAttributeType vertexAttributeType{ VertexAttributeType::NONE };
        std::string name;
        bool normalized{ false };
        uint32_t size{ 0 };
        uint64_t offset{ 0 };
    };
}
