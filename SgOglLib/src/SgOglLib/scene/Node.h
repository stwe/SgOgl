#pragma once

// todo delete raw pointer

#include <vector>
#include <string>
#include <memory>
#include "math/Transform.h"

namespace sg::ogl::resource
{
    class Mesh;
    struct Material;
}

namespace sg::ogl::scene
{
    class Node
    {
    public:
        using ChildrenContainer = std::vector<Node*>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        MeshSharedPtr mesh;
        MaterialSharedPtr material;
        int32_t instanceCount{ 0 };

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

        const std::string& GetUuid() const noexcept;
        const std::string& GetDebugName() const noexcept;
        Node* GetParent() const;
        ChildrenContainer& GetChildren() noexcept;
        const ChildrenContainer& GetChildren() const noexcept;
        math::Transform& GetLocalTransform() noexcept;
        glm::mat4 GetWorldMatrix() const;
        bool HasChildren() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetDebugName(const std::string& t_debugName);

        //-------------------------------------------------
        // Parent - Children
        //-------------------------------------------------

        void SetParent(Node* t_parentNode);
        void AddChild(Node* t_childNode);

        //-------------------------------------------------
        // Transform
        //-------------------------------------------------

        void CalcWorldMatrix();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Init();
        virtual void Input();
        virtual void Update(double t_dt);
        virtual void Render();

    protected:

    private:
        /**
         * @brief Each node is uniquely identified by a stringified UUID.
         */
        std::string m_uuid;

        /**
         * @brief A debug name.
         */
        std::string m_debugName;

        /**
         * @brief The parent node.
         */
        Node* m_parent{ nullptr };

        /**
         * @brief Each node can have any number of children.
         */
        ChildrenContainer m_children;

        /**
         * @brief Local transform.
         */
        math::Transform m_localTransform;

        /**
         * @brief The world matrix places the node in the world.
         */
        glm::mat4 m_worldMatrix{ glm::mat4(1.0f) };
    };
}
