#pragma once

#include <map>
#include <string>
#include <memory>
#include "Node.h"

namespace sg::ogl::scene
{
    class Component;
    class Scene;

    class SG_OGL_API Entity : public Node
    {
    public:
        using ComponentKey = std::string;
        using ComponentSharedPtr = std::shared_ptr<Component>;
        using ComponentContainer = std::map<ComponentKey, ComponentSharedPtr>;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ComponentContainer& GetComponents();
        Component& GetComponent(const ComponentKey& t_componentKey);
        Scene* GetParentScene();

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void AddComponent(const ComponentKey& t_componentKey, const ComponentSharedPtr& t_component);
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
