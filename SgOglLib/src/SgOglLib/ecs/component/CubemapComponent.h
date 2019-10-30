// This file is part of the SgOgl package.
// 
// Filename: CubemapComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>

namespace sg::ogl::ecs::component
{
    struct CubemapComponent
    {
        uint32_t cubemapId;
    };
}
