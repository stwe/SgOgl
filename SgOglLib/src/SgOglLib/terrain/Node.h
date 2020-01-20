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
#include <string>
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
    class Node
    {
    public:
        using NodeUniquePtr = std::unique_ptr<Node>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        std::string name;

        Node* parent{ nullptr };
        std::vector<NodeUniquePtr> children;
        Transform localTransform;
        Transform worldTransform;

        bool isLeaf{ true };
        int lod{ 0 };
        glm::vec2 location{ glm::vec2(0.0f) };
        glm::vec3 center{ glm::vec3(0.0f) };
        glm::vec2 index{ glm::vec2(0.0f) };
        float gap{ 1.0f };
        //               Lod        0     1    2    3    4    5   6  7
        std::vector<int> lodRanges{ 1740, 870, 300, 200, 100, 50, 0, 0 };

        glm::vec3 color{ glm::vec3(0.0f, 1.0f, 0.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Node(scene::Scene* t_scene, const std::string& t_name, int t_lod, const glm::vec2& t_location, const glm::vec2& t_index);

        Node(const Node& t_other) = delete;
        Node(Node&& t_other) noexcept = delete;
        Node& operator=(const Node& t_other) = delete;
        Node& operator=(Node&& t_other) noexcept = delete;

        virtual ~Node() noexcept = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(resource::ShaderProgram& t_shaderProgram, const MeshSharedPtr& t_patchMesh);

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

        //-------------------------------------------------
        // Add / Remove
        //-------------------------------------------------

        void AddChild(NodeUniquePtr t_child);
        void Add4Children(int t_lod);

        void RemoveChildren();

    protected:

    private:
        scene::Scene* m_scene{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void ComputeCenterPosition();
    };
}
