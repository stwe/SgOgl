// This file is part of the SgOgl package.
// 
// Filename: TerrainConfig.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Core.h"
#include "Log.h"

namespace sg::ogl::terrain
{
    constexpr std::array POW2{
        1, 2, 4, 8, 16,
        32, 64, 128, 256, 512
    };

    constexpr std::array POW2_F{
        1.0f, 2.0f, 4.0f, 8.0f, 16.0f,
        32.0f, 64.0f, 128.0f, 256.0f, 512.0f
    };

    struct TerrainConfig
    {
        float scaleXz{ 1.0f };
        float scaleY{ 1.0f };
        int rootNodes{ 2 };

        std::string heightmapFilePath;

        int tessellationFactor{ 600 };
        float tessellationSlope{ 1.8f };
        float tessellationShift{ 0.1f };

        bool morphingEnabled{ true };
        bool tessellationEnabled{ true };

        std::vector<int> lodRanges;
        std::vector<int> lodMorphingArea;

        void Init()
        {
            SG_OGL_CORE_ASSERT(!lodRanges.empty(), "[TerrainConfig::Init()] There are no values for the Lod Ranges.")
            SG_OGL_CORE_ASSERT(lodRanges.size() <= 10, "[TerrainConfig::Init()] There are to many values for the Lod Ranges.")

            for (auto i{ 0u }; i < lodRanges.size(); ++i)
            {
                if (lodRanges[i] == 0)
                {
                    lodMorphingArea.push_back(0);
                }
                else
                {
                    lodMorphingArea.push_back(
                        lodRanges[i] - (static_cast<int>(scaleXz) / rootNodes) / POW2[i + 1]
                    );
                }
            }
        }
    };
}
