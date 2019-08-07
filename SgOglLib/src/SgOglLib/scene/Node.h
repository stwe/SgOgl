#pragma once

#include <string>
#include <vector>

namespace sg::ogl::resource
{
    class Mesh;
    class Material;
}

namespace sg::ogl::scene
{
    class Node
    {
    public:
        using NodeChildren = std::vector<Node*>;

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

        std::string GetName() const;
        Node* GetParent() const;
        NodeChildren& GetChildren();

        //-------------------------------------------------
        // Children
        //-------------------------------------------------

        void AddChild(Node* t_childNode);

    protected:

    private:
        std::string m_name;
        Node* m_parent{ nullptr };
        NodeChildren m_children;
    };
}
