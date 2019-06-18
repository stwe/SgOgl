#pragma once

namespace sg::ogl::buffer
{
    template <typename T>
    struct VertexAttribute
    {
        VertexAttribute() = default;

        VertexAttribute(std::string t_name, int32_t t_numberOfComponents, bool t_normalized = false)
            : name{ std::move( t_name ) }
            , numberOfComponents{ t_numberOfComponents}
            , normalized{ t_normalized }
        {}

        ~VertexAttribute() = default;

        /**
         * @brief The name of the attribute.
         */
        std::string name;

        /**
         * @brief Specifies the number of components.
         */
        int32_t numberOfComponents{ 0 };

        /**
         * @brief specifies whether fixed-point data values should be normalized.
         */
        bool normalized{ false };

        static constexpr auto sizeOfAttributeType{ sizeof(T) };

        int32_t offset{ 0 };
    };
}
