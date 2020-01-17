// This file is part of the SgOgl package.
// 
// Filename: Node.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "Node.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::terrain::Node::Node(const int t_lod, const glm::vec2& t_location, const glm::vec2& t_index)
    : lod{ t_lod }
    , location{ t_location }
    , index{ t_index }
{
    isLeaf = true;
    gap = 1.0f / powf(2.0f, static_cast<float>(lod));
    localScale = glm::vec2(gap);
    localPosition = location;
}

//-------------------------------------------------
// Add
//-------------------------------------------------

void sg::ogl::terrain::Node::AddChild(Node* t_child)
{
    t_child->parent = this;
    children.push_back(t_child);
    isLeaf = false;
}

void sg::ogl::terrain::Node::Add4Children(int t_lod)
{
    if (children.empty())
    {
        for (auto i{ 0 }; i < 2; ++i)
        {
            for (auto j{ 0 }; j < 2; ++j)
            {
                const auto loc{ location + glm::vec2(i * gap / 2.0f, j * gap / 2.0f) };
                AddChild(new Node(lod, loc, glm::vec2(i, j)));
            }
        }
    }
}
