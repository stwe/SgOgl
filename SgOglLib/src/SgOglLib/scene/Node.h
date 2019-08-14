#pragma once

// todo delete raw pointer

#include <vector>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class Mesh;
    struct Material;
}

namespace sg::ogl::scene
{
    class MeshLoader;
    class Scene;

    class SG_OGL_API Node
    {
    public:
        friend MeshLoader; // to use private functions in MeshLoader
        friend Scene;      // to use private functions in Scene

        using ChildrenContainer = std::vector<Node*>;

        resource::Mesh* mesh{ nullptr };
        resource::Material* material{ nullptr };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Node();

        Node(const Node& t_other) = delete;
        Node(Node&& t_other) noexcept = delete;
        Node& operator=(const Node& t_other) = delete;
        Node& operator=(Node&& t_other) noexcept = delete;

        virtual ~Node() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        std::string GetUuid() const;
        Node* GetParent() const;
        const ChildrenContainer& GetChildren() const;

    protected:

    private:
        std::string m_uuid;

        Node* m_parent{ nullptr };
        ChildrenContainer m_children;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParent(Node* t_parentNode);

        //-------------------------------------------------
        // Children
        //-------------------------------------------------

        void AddChild(Node* t_childNode);
    };
}
