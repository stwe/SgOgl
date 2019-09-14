#include "Component.h"

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Entity* sg::ogl::scene::Component::GetParentEntity() const
{
    return m_parentEntity;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Component::SetParentEntity(Entity* t_entity)
{
    m_parentEntity = t_entity;
}
