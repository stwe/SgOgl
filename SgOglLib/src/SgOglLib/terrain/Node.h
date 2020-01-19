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
#include <iostream>
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
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        std::string name;

        Node* parent{ nullptr };
        std::vector<Node*> children;
        Transform localTransform;
        Transform worldTransform;

        bool isLeaf{ true };
        int lod{ 0 };
        glm::vec2 location{ glm::vec2(0.0f) };
        glm::vec3 center{ glm::vec3(0.0f) };
        glm::vec2 index{ glm::vec2(0.0f) };
        float gap{ 1.0f };
        //               Lod        0     1     2     3    4    5    6  7
        std::vector<int> lodRanges{ 2000, 1500, 1000, 800, 400, 200, 0, 0 };

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
        // Debug
        //-------------------------------------------------

        static void Debug(Node* t_node)
        {
            std::cout << "\n";
            std::cout << "isLeaf: " << t_node->isLeaf << "\n";
            std::cout << "Numer of children: " << t_node->children.size() << "\n";
            std::cout << "location x: " << t_node->location.x << " location y:" << t_node->location.y << "\n";
            std::cout << "center x: " << t_node->center.x << " center y:" << t_node->center.y << " center z:" << t_node->center.z << "\n";
            std::cout << "index x: " << t_node->index.x << " index y:" << t_node->index.y << "\n";
            std::cout << "gap: " << t_node->gap << "\n";

            for (auto* child : t_node->children)
            {
                Debug(child);
            }
        }

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(resource::ShaderProgram& t_shaderProgram, const std::shared_ptr<resource::Mesh>& t_patchMesh);

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        void Update();

        //-------------------------------------------------
        // Add / Remove
        //-------------------------------------------------

        void AddChild(Node* t_child);
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
