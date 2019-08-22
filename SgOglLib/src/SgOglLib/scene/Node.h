#pragma once

// todo delete raw pointer

#include <vector>
#include <string>
#include "Core.h"
#include "math/Transform.h"

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
        const ChildrenContainer& GetChildren() const noexcept;
        math::Transform& GetLocalTransform() noexcept;
        glm::mat4 GetWorldMatrix() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetDebugName(const std::string& t_debugName);
        void SetParent(Node* t_parentNode);

        //-------------------------------------------------
        // Children
        //-------------------------------------------------

        void AddChild(Node* t_childNode);

        //-------------------------------------------------
        // Transform
        //-------------------------------------------------

        void CalcWorldMatrix();

    protected:

    private:
        /**
         * @brief Each node is uniquely identified by a stringified UUID.
         */
        std::string m_uuid;

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

        glm::mat4 m_worldMatrix{ glm::mat4(1.0f) };

        /**
         * @brief Mark the current node's tranform as dirty if it needs
         *        to be re-calculated this frame.
         */
        bool m_dirty{ false };
    };
}
