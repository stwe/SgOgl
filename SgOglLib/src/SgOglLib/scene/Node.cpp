#include "Node.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Node::Node()
{
}

sg::ogl::scene::Node::~Node() noexcept
{
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

std::string sg::ogl::scene::Node::GetName() const
{
    return m_name;
}

sg::ogl::scene::Node* sg::ogl::scene::Node::GetParent() const
{
    return m_parent;
}

sg::ogl::scene::Node::NodeChildren& sg::ogl::scene::Node::GetChildren()
{
    return m_children;
}

//-------------------------------------------------
// Children
//-------------------------------------------------

void sg::ogl::scene::Node::AddChild(Node* t_childNode)
{
    t_childNode->m_parent = this;
    m_children.push_back(t_childNode);
}
