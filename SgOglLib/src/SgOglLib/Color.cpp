#include "Color.h"

sg::ogl::Color::Color(const uint8_t t_red, const uint8_t t_green, const uint8_t t_blue, const uint8_t t_alpha)
    : r{ t_red }
    , g{ t_green }
    , b{ t_blue }
    , a{ t_alpha }
{}

sg::ogl::Color::Color(uint32_t color)
    : r( (color & 0xff000000) >> 24 )
    , g( (color & 0x00ff0000) >> 16 )
    , b( (color & 0x0000ff00) >> 8  )
    , a( (color & 0x000000ff) >> 0  )
{}

uint32_t sg::ogl::Color::ToInteger() const
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}
