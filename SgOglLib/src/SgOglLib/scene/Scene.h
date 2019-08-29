#pragma once

#include <memory>
#include "Core.h"

namespace sg::ogl::resource
{
    class Model;
    struct Material;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;
    class Renderer;

    class SG_OGL_API Scene
    {
    public:
        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        Scene(Renderer& t_renderer, camera::LookAtCamera& t_camera);

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Renderer& GetRenderer() noexcept;
        const Renderer& GetRenderer() const noexcept;

        camera::LookAtCamera& GetCamera() noexcept;
        const camera::LookAtCamera& GetCamera() const noexcept;

        Node* GetRoot() const;

        //-------------------------------------------------
        // Scene objects (Nodes)
        //-------------------------------------------------

        // Should we pass a shared_ptr by reference or by value?
        // https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value/8741626

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);

        void AddNodeToRoot(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render();

    protected:

    private:
        Renderer& m_renderer;
        camera::LookAtCamera& m_camera;

        Node* m_rootNode{ nullptr };
    };
}
