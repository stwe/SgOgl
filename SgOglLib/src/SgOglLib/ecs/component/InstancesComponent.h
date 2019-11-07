// This file is part of the SgOgl package.
// 
// Filename: InstancesComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>

namespace sg::ogl::ecs::component
{
    struct InstancesComponent
    {
        uint32_t instances{ 0 };
    };
}
