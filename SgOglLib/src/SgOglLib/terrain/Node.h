// This file is part of the SgOgl package.
// 
// Filename: Node.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <vector>
#include <memory>
#include "Transform.h"

namespace sg::ogl::resource
{
    class Mesh;
    class ShaderProgram;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::terrain
{
    struct TerrainConfig;

    class Node
    {
    public:
        using NodeUniquePtr = std::unique_ptr<Node>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;
        using TerrainConfigSharedPtr = std::shared_ptr<TerrainConfig>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Node(
            scene::Scene* t_scene,
            const TerrainConfigSharedPtr& t_terrainConfig,
            int t_lod,
            const glm::vec2& t_location,
            const glm::vec2& t_index
        );

        Node(const Node& t_other) = delete;
        Node(Node&& t_other) noexcept = delete;
        Node& operator=(const Node& t_other) = delete;
        Node& operator=(Node&& t_other) noexcept = delete;

        virtual ~Node() noexcept = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(resource::ShaderProgram& t_shaderProgram, const MeshSharedPtr& t_patchMesh);
        void Update();

        //-------------------------------------------------
        // Debug
        //-------------------------------------------------

        void NodesCount(int& t_value)
        {
            for (const auto& child : m_children)
            {
                t_value++;
                child->NodesCount(t_value);
            }
        }

        //-------------------------------------------------
        // Add / Remove
        //-------------------------------------------------

        void AddChild(NodeUniquePtr t_child);

    protected:
        std::vector<NodeUniquePtr> m_children;

        TerrainConfigSharedPtr m_terrainConfig;

    private:
        scene::Scene* m_scene{ nullptr };

        Node* m_parent{ nullptr };

        int m_lod{ 0 };

        glm::vec2 m_location{ glm::vec2(0.0f) };
        glm::vec2 m_index{ glm::vec2(0.0f) };
        glm::vec3 m_center{ glm::vec3(0.0f) };

        bool m_isLeaf{ true };

        float m_gap{ 1.0f };

        Transform m_localTransform;
        Transform m_worldTransform;

        glm::vec3 m_color{ glm::vec3(0.0f, 1.0f, 0.0f) };

        //-------------------------------------------------
        // Add / Remove
        //-------------------------------------------------

        void Add4Children(int t_lod);
        void RemoveChildren();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void ComputeCenterPosition();
    };
}
