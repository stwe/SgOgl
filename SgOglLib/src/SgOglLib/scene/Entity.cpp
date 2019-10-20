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

sg::ogl::scene::Entity::ComponentSharedPtr& sg::ogl::scene::Entity::GetComponentSharedPtr(Component::Type t_type)
{
    return m_components.at(t_type);
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

void sg::ogl::scene::Entity::Init()
{
    for (auto& component : m_components)
    {
        component.second->Init();
    }
}

void sg::ogl::scene::Entity::Input()
{
    for (auto& component : m_components)
    {
        component.second->Input();
    }

    Node::Input();
}

void sg::ogl::scene::Entity::Update(const double t_dt)
{
    for (auto& component : m_components)
    {
        component.second->Update(t_dt);
    }

    Node::Update(t_dt);
}

void sg::ogl::scene::Entity::Render()
{
    for (auto& component : m_components)
    {
        component.second->Render();
    }

    Node::Render();
}
