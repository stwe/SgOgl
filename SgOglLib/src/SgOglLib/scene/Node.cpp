#include <uuid.h>
#include "Node.h"
#include "Log.h"

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
    for (auto* child : m_children)
    {
        // It should not be possible that a scene node is childed by more than one
        // parent, thus we don't need to care about deleting dangling pointers.
        delete child;
    }
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

const sg::ogl::scene::Node::ChildrenContainer& sg::ogl::scene::Node::GetChildren() const noexcept
{
    return m_children;
}

sg::ogl::math::Transform& sg::ogl::scene::Node::GetLocalTransform() noexcept
{
    return m_localTransform;
}

glm::mat4 sg::ogl::scene::Node::GetWorldMatrix() const
{
    return m_worldMatrix;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Node::SetDebugName(const std::string& t_debugName)
{
    m_debugName = t_debugName;
}

void sg::ogl::scene::Node::SetParent(Node* t_parentNode)
{
    m_parent = t_parentNode;
}

//-------------------------------------------------
// Children
//-------------------------------------------------

void sg::ogl::scene::Node::AddChild(Node* t_childNode)
{
    SG_OGL_CORE_ASSERT(!t_childNode->m_parent, "[Node::AddChild()] Scene nodes aren't allowed to exist under multiple parents.")

    t_childNode->SetParent(this);
    m_children.push_back(t_childNode);
}

//-------------------------------------------------
// Transform
//-------------------------------------------------

void sg::ogl::scene::Node::CalcWorldMatrix()
{
    if (m_parent)
    {
        m_worldMatrix = m_parent->GetWorldMatrix() * m_localTransform.GetModelMatrix();
    }
    else
    {
        m_worldMatrix = m_localTransform.GetModelMatrix();
    }

    for (auto* child : m_children)
    {
        child->CalcWorldMatrix();
    }
}
