// This file is part of the SgOgl package.
// 
// Filename: WaterComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::water
{
    class Water;
}

namespace sg::ogl::ecs::component
{
    struct WaterComponent
    {
        std::shared_ptr<water::Water> water;
    };
}
