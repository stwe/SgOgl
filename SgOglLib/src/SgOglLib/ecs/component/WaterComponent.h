// This file is part of the SgOgl package.
// 
// Filename: WaterComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>

namespace sg::ogl::ecs::component
{
    struct WaterComponent
    {
        uint32_t reflectionTextureId{ 0 };
        uint32_t refractionTextureId{ 0 };
        uint32_t dudvTextureId{ 0 };
        uint32_t normalTextureId{ 0 };
        uint32_t depthTextureId{ 0 };

        float waveSpeed{ 0.04f };
        float moveFactor{ 0.0f };
    };
}
