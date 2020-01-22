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
#include "TerrainConfig.h"
#include "scene/Scene.h"

namespace sg::ogl::terrain
{
    class TerrainQuadtree : public Node
    {
    public:
        static constexpr auto START_LOD{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit TerrainQuadtree(scene::Scene* t_scene, const TerrainConfigSharedPtr& t_terrainConfig)
            : Node(t_scene, t_terrainConfig, START_LOD, glm::vec2(0.0f), glm::vec2(0.0f))
        {
            for (auto i{ 0 }; i < m_terrainConfig->rootNodes; ++i)
            {
                for (auto j{ 0 }; j < m_terrainConfig->rootNodes; ++j)
                {
                    AddChild(std::make_unique<Node>(
                        t_scene,
                        m_terrainConfig,
                        START_LOD,
                        glm::vec2(i / static_cast<float>(m_terrainConfig->rootNodes), j / static_cast<float>(m_terrainConfig->rootNodes)),
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
            for (const auto& child : m_children)
            {
                child->Update();
            }
        }

    protected:

    private:

    };
}
