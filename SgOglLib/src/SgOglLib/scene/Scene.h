#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Application.h"
#include "Entity.h"
#include "resource/Model.h"
#include "resource/Material.h"
#include "resource/ShaderProgram.h"
#include "terrain/Terrain.h"

namespace sg::ogl::light
{
    struct DirectionalLight;
    struct PointLight;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;

    class Scene
    {
    public:
        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;

        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        explicit Scene(Application* t_application);

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        camera::LookAtCamera& GetCurrentCamera() noexcept;
        const camera::LookAtCamera& GetCurrentCamera() const noexcept;

        light::DirectionalLight& GetDirectionalLight() noexcept;
        const light::DirectionalLight& GetDirectionalLight() const noexcept;

        light::PointLight& GetPointLight() noexcept;
        const light::PointLight& GetPointLight() const noexcept;

        bool IsDirectionalLight() const;
        bool IsPointLight() const;

        Node* GetRoot() const;

        Application* GetApplicationContext() const;

        glm::vec4 GetCurrentClipPlane() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);
        void SetCurrentClipPlane(const glm::vec4& t_currentClipPlane);

        //-------------------------------------------------
        // Vertex attribute
        //-------------------------------------------------

        static void SetInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);

        /*
        void AddWaterComponent(
            Entity* t_entity,
            const uint32_t t_reflectionTextureId,
            const uint32_t t_refractionTextureId,
            const uint32_t t_dudvTextureId,
            const uint32_t t_normalTextureId,
            const uint32_t t_depthTextureId
        ) const
        {
            auto waterComponentUniquePtr{ std::make_unique<component::WaterComponent>() };
            waterComponentUniquePtr->reflectionTextureId = t_reflectionTextureId;
            waterComponentUniquePtr->refractionTextureId = t_refractionTextureId;
            waterComponentUniquePtr->dudvTextureId = t_dudvTextureId;
            waterComponentUniquePtr->normalTextureId = t_normalTextureId;
            waterComponentUniquePtr->depthTextureId = t_depthTextureId;

            t_entity->AddComponent(Component::Type::WATER, std::move(waterComponentUniquePtr));
        }
        */

        void AddNodeToRoot(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render() const;
        void Render(Node* t_node) const;

    protected:

    private:
        CameraSharedPtr m_currentCamera;

        DirectionalLightSharedPtr m_directionalLight;
        PointLightSharedPtr m_pointLight;

        Node* m_rootNode{ nullptr };

        Application* m_application{ nullptr };

        glm::vec4 m_currentClipPlane{ glm::vec4(0.0f) };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);
    };
}
