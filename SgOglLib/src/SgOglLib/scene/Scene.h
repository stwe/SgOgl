#pragma once

#include <memory>
#include "Core.h"

// Should we pass a shared_ptr by reference or by value?
// https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value/8741626

namespace sg::ogl::light
{
    struct DirectionalLight;
    struct PointLight;
}

namespace sg::ogl::resource
{
    class Model;
    struct Material;
    class Skybox;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;
    class Renderer;
    class SkyboxRenderer;

    class SG_OGL_API Scene
    {
    public:
        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;
        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;
        using RendererSharedPtr = std::shared_ptr<Renderer>;
        using SkyboxRendererSharedPtr = std::shared_ptr<SkyboxRenderer>;
        using SkyboxSharedPtr = std::shared_ptr<resource::Skybox>;
        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        Scene(const RendererSharedPtr& t_renderer, const SkyboxRendererSharedPtr& t_skyboxRenderer);

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

        SkyboxRenderer& GetSkyboxRenderer() noexcept;
        const SkyboxRenderer& GetSkyboxRenderer() const noexcept;

        camera::LookAtCamera& GetCurrentCamera() noexcept;
        const camera::LookAtCamera& GetCurrentCamera() const noexcept;

        light::DirectionalLight& GetDirectionalLight() noexcept;
        const light::DirectionalLight& GetDirectionalLight() const noexcept;

        light::PointLight& GetPointLight() noexcept;
        const light::PointLight& GetPointLight() const noexcept;

        bool IsDirectionalLight() const;
        bool IsPointLight() const;

        Node* GetRoot() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetSkybox(const SkyboxSharedPtr& t_skybox);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);

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
        SkyboxRendererSharedPtr m_skyboxRenderer;

        CameraSharedPtr m_currentCamera;
        SkyboxSharedPtr m_skybox;

        DirectionalLightSharedPtr m_directionalLight;
        PointLightSharedPtr m_pointLight;

        Node* m_rootNode{ nullptr };
    };
}
