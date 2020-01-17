// This file is part of the SgOgl package.
// 
// Filename: TerrainQuadtree.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Node.h"

namespace sg::ogl::terrain
{
    class TerrainQuadtree : public Node
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainQuadtree() = default;

        TerrainQuadtree(const TerrainQuadtree& t_other) = delete;
        TerrainQuadtree(TerrainQuadtree&& t_other) noexcept = delete;
        TerrainQuadtree& operator=(const TerrainQuadtree& t_other) = delete;
        TerrainQuadtree& operator=(TerrainQuadtree&& t_other) noexcept = delete;

        ~TerrainQuadtree() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------



    protected:

    private:

    };
}
