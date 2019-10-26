// This file is part of the SgOgl package.
// 
// Filename: BufferLayout.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <vector>
#include <cstdint>
#include "VertexAttribute.h"
#include "Core.h"

namespace sg::ogl::buffer
{
    class SG_OGL_API BufferLayout
    {
    public:
        using AttributeContainer = std::vector<VertexAttribute>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BufferLayout() = default;

        BufferLayout(const std::initializer_list<VertexAttribute>& t_vertexAttributes);

        BufferLayout(const BufferLayout& t_other) = delete;
        BufferLayout(BufferLayout&& t_other) noexcept = delete;
        BufferLayout& operator=(const BufferLayout& t_other) = delete;
        BufferLayout& operator=(BufferLayout&& t_other) noexcept = delete;

        ~BufferLayout() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const AttributeContainer& GetAttributes() const noexcept;
        int32_t GetStride() const;
        uint32_t GetNumberOfFloats() const;

        static uint32_t GetVertexAttributeTypeSize(VertexAttributeType t_vertexAttributeType);
        static uint32_t GetOpenGlType(VertexAttributeType t_vertexAttributeType);

    protected:

    private:
        AttributeContainer m_vertexAttributes;
        int32_t m_stride{ 0 };
        uint32_t m_numberOfFloats{ 0 };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void Init();
    };
}
