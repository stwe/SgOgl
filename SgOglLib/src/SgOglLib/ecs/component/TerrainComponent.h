// This file is part of the SgOgl package.
// 
// Filename: TerrainComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::ecs::component
{
    struct TerrainComponent
    {
        std::shared_ptr<terrain::Terrain> terrain;
    };
}
