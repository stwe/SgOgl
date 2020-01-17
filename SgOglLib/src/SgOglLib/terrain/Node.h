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
#include <glm/vec2.hpp>

namespace sg::ogl::terrain
{
    class Node
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        Node* parent{ nullptr };
        std::vector<Node*> children;

        bool isLeaf{ true };

        int lod{ 0 };
        float gap{ 1.0f };
        glm::vec2 location{ glm::vec2(0.0f) };
        glm::vec2 index{ glm::vec2(0.0f) };

        glm::vec2 localScale{ glm::vec2(1.0f) };
        glm::vec2 localPosition{ glm::vec2(0.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Node() = default;

        Node(int t_lod, const glm::vec2& t_location, const glm::vec2& t_index);

        Node(const Node& t_other) = delete;
        Node(Node&& t_other) noexcept = delete;
        Node& operator=(const Node& t_other) = delete;
        Node& operator=(Node&& t_other) noexcept = delete;

        virtual ~Node() noexcept = default;

        //-------------------------------------------------
        // Add
        //-------------------------------------------------

        void AddChild(Node* t_child);
        void Add4Children(int t_lod);

    protected:

    private:

    };
}
