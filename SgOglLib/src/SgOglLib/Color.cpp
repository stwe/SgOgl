// This file is part of the SgOgl package.
// 
// Filename: Color.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Color.h"

sg::ogl::Color::Color(const uint8_t t_red, const uint8_t t_green, const uint8_t t_blue, const uint8_t t_alpha)
    : r{ t_red }
    , g{ t_green }
    , b{ t_blue }
    , a{ t_alpha }
{}

sg::ogl::Color::Color(const uint32_t t_color)
    : r( (t_color & 0xff000000) >> 24 )
    , g( (t_color & 0x00ff0000) >> 16 )
    , b( (t_color & 0x0000ff00) >> 8  )
    , a( (t_color & 0x000000ff) >> 0  )
{}

uint32_t sg::ogl::Color::ToInteger() const
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}
