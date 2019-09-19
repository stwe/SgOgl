#pragma once

#include <map>
#include <memory>
#include "Node.h"
#include "Component.h"

namespace sg::ogl::scene
{
    class Scene;

    class SG_OGL_API Entity : public Node
    {
    public:
        using ComponentSharedPtr = std::shared_ptr<Component>;
        using ComponentContainer = std::map<Component::Type, ComponentSharedPtr>;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ComponentContainer& GetComponents();
        Component& GetComponent(Component::Type t_type);
        Scene* GetParentScene() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void AddComponent(Component::Type t_type, const ComponentSharedPtr& t_component);
        void SetParentScene(Scene* t_scene);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input() override;
        void Update() override;
        void Render() override;

    protected:

    private:
        ComponentContainer m_components;
        Scene* m_parentScene{ nullptr };
    };
}
