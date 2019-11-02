#pragma once

#include <cstdint>

namespace sg::ogl
{
    struct Color
    {
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        /**
         * @brief Default constructor.
         * It is equivalent to Color(0, 0, 0, 255).
         */
        Color() = default;

        /**
         * @brief Construct the color from its 4 RGBA components.
         *
         * @param t_red Red component (in the range [0, 255]).
         * @param t_green Green component (in the range [0, 255]).
         * @param t_blue Blue component (in the range [0, 255]).
         * @param t_alpha Alpha (opacity) component (in the range [0, 255]).
         */
        Color(uint8_t t_red, uint8_t t_green, uint8_t t_blue, uint8_t t_alpha = 255);

        /**
         * @brief Construct the color from 32-bit unsigned integer.
         *
         * @param t_color Color Number containing the RGBA components (in that order).
         */
        explicit Color(uint32_t t_color);

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * @brief Retrieve the color as a 32-bit unsigned integer.
         * @return Color represented as a 32-bit unsigned integer
         */
        uint32_t ToInteger() const;

        //-------------------------------------------------
        // Color components
        //-------------------------------------------------

        uint8_t r{ 0 };
        uint8_t g{ 0 };
        uint8_t b{ 0 };
        uint8_t a{ 255 };

        //-------------------------------------------------
        // Predefined colors
        //-------------------------------------------------

        // @see https://stackoverflow.com/questions/29772450/c-how-to-export-a-static-class-member-from-a-dll

        static Color Black()
        {
            static const auto COL = Color(0, 0, 0);
            return COL;
        }

        static Color White()
        {
            static const auto COL = Color(255, 255, 255);
            return COL;
        }

        static Color Red()
        {
            static const auto COL = Color(255, 0, 0);
            return COL;
        }

        static Color Green()
        {
            static const auto COL = Color(0, 255, 0);
            return COL;
        }

        static Color Blue()
        {
            static const auto COL = Color(0, 0, 255);
            return COL;
        }

        static Color Yellow()
        {
            static const auto COL = Color(255, 255, 0);
            return COL;
        }

        static Color Magenta()
        {
            static const auto COL = Color(255, 0, 255);
            return COL;
        }

        static Color Cyan()
        {
            static const auto COL = Color(0, 255, 255);
            return COL;
        }

        static Color CornflowerBlue()
        {
            static const auto COL = Color(39, 58, 93);
            return COL;
        }
    };
}
