// This file is part of the SgOgl package.
// 
// Filename: TerrainConfig.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

namespace sg::ogl::terrain
{
    struct TerrainConfig
    {
        float scaleXz{ 400.0f };   // 400
        float scaleY{ 1.0f };      // 1
        int rootNodes{ 2 };        // 2
    };
}
