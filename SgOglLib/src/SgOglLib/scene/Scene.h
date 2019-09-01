#pragma once

#include <memory>
#include "Core.h"

// Should we pass a shared_ptr by reference or by value?
// https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value/8741626

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
        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;
        using RendererSharedPtr = std::shared_ptr<Renderer>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        explicit Scene(const RendererSharedPtr& t_renderer);

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

        camera::LookAtCamera& GetCurrentCamera() noexcept;
        const camera::LookAtCamera& GetCurrentCamera() const noexcept;

        Node* GetRoot() const;

        //-------------------------------------------------
        // Camera
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);

        //-------------------------------------------------
        // Scene objects (Nodes)
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);
        void AddNodeToRoot(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render();

    protected:

    private:
        RendererSharedPtr m_renderer;
        CameraSharedPtr m_currentCamera;

        Node* m_rootNode{ nullptr };
    };
}
