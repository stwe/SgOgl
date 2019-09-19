#include "Entity.h"
#include "Component.h"

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Entity::ComponentContainer& sg::ogl::scene::Entity::GetComponents()
{
    return m_components;
}

sg::ogl::scene::Component& sg::ogl::scene::Entity::GetComponent(const Component::Type t_type)
{
    return *m_components.at(t_type);
}

sg::ogl::scene::Scene* sg::ogl::scene::Entity::GetParentScene() const
{
    return m_parentScene;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Entity::AddComponent(Component::Type t_type, const ComponentSharedPtr& t_component)
{
    t_component->SetParentEntity(this);
    m_components.emplace(t_type, t_component);
}

void sg::ogl::scene::Entity::SetParentScene(Scene* t_scene)
{
    m_parentScene = t_scene;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Entity::Input()
{
    for (auto& component : m_components)
    {
        component.second->Input();
    }

    Node::Input();
}

void sg::ogl::scene::Entity::Update()
{
    for (auto& component : m_components)
    {
        component.second->Update();
    }

    Node::Update();
}

void sg::ogl::scene::Entity::Render()
{
    for (auto& component : m_components)
    {
        component.second->Render();
    }

    Node::Render();
}
