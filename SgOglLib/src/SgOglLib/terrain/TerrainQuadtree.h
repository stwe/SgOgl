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
#include "scene/Scene.h"

namespace sg::ogl::terrain
{
    class TerrainQuadtree : public Node
    {
    public:
        static constexpr auto SCALE_XZ{ 400.0f };
        static constexpr auto SCALE_Y{ 1.0f };
        static constexpr auto ROOT_NODES{ 2 };
        static constexpr auto START_LOD{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit TerrainQuadtree(scene::Scene* t_scene)
            : Node(t_scene, "terrain_quadtree", START_LOD, glm::vec2(0.0f), glm::vec2(0.0f))
        {
            for (auto i{ 0 }; i < ROOT_NODES; ++i)
            {
                for (auto j{ 0 }; j < ROOT_NODES; ++j)
                {
                    AddChild(std::make_unique<Node>(
                        t_scene,
                        "root_node",
                        START_LOD,
                        glm::vec2(i / static_cast<float>(ROOT_NODES), j / static_cast<float>(ROOT_NODES)),
                        glm::vec2(i, j)
                        )
                    );
                }
            }
        }

        TerrainQuadtree(const TerrainQuadtree& t_other) = delete;
        TerrainQuadtree(TerrainQuadtree&& t_other) noexcept = delete;
        TerrainQuadtree& operator=(const TerrainQuadtree& t_other) = delete;
        TerrainQuadtree& operator=(TerrainQuadtree&& t_other) noexcept = delete;

        virtual ~TerrainQuadtree() noexcept = default;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void UpdateQuadtree()
        {
            for (const auto& child : children)
            {
                child->Update();
            }
        }

    protected:

    private:

    };
}
