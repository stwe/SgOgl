#pragma once

#include <vector>
#include <cstdint>
#include "Core.h"

namespace sg::ogl::buffer
{
    enum class VertexAttributeType;
    struct VertexAttribute;

    class SG_OGL_API BufferLayout
    {
    public:
        using Attributes = std::vector<VertexAttribute>;

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

        const Attributes& GetAttributes() const noexcept;
        int32_t GetStride() const;

        static uint32_t GetVertexAttributeTypeSize(VertexAttributeType t_vertexAttributeType);
        static uint32_t GetOpenGlType(VertexAttributeType t_vertexAttributeType);

    protected:

    private:
        Attributes m_vertexAttributes;
        int32_t m_stride{ 0 };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void CalculateOffsetsAndStride();
    };
}
