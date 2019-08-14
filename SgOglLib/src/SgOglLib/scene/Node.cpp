#include <uuid.h>
#include "Node.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Node::Node()
{
    auto const uuid{ uuids::uuid_system_generator{}() };
    m_uuid = to_string(uuid);
}

sg::ogl::scene::Node::~Node() noexcept
{
    // todo
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

std::string sg::ogl::scene::Node::GetUuid() const
{
    return m_uuid;
}

sg::ogl::scene::Node* sg::ogl::scene::Node::GetParent() const
{
    return m_parent;
}

const sg::ogl::scene::Node::ChildrenContainer& sg::ogl::scene::Node::GetChildren() const
{
    return m_children;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Node::SetParent(Node* t_parentNode)
{
    m_parent = t_parentNode;
}

//-------------------------------------------------
// Children
//-------------------------------------------------

void sg::ogl::scene::Node::AddChild(Node* t_childNode)
{
    t_childNode->SetParent(this);
    m_children.push_back(t_childNode);
}
