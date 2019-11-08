// This file is part of the SgOgl package.
// 
// Filename: Scene.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>
#include <glm/vec4.hpp>

namespace sg::ogl
{
    class Application;
}

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
    class Scene
    {
    public:
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

        Application* GetApplicationContext() const;

        camera::LookAtCamera& GetCurrentCamera() noexcept;
        const camera::LookAtCamera& GetCurrentCamera() const noexcept;

        light::DirectionalLight& GetDirectionalLight() noexcept;
        const light::DirectionalLight& GetDirectionalLight() const noexcept;

        light::PointLight& GetPointLight() noexcept;
        const light::PointLight& GetPointLight() const noexcept;

        bool IsDirectionalLight() const;
        bool IsPointLight() const;

        glm::vec4 GetCurrentClipPlane() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);
        void SetCurrentClipPlane(const glm::vec4& t_currentClipPlane);

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

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

    protected:

    private:
        Application* m_application{ nullptr };

        CameraSharedPtr m_currentCamera;

        DirectionalLightSharedPtr m_directionalLight;
        PointLightSharedPtr m_pointLight;

        glm::vec4 m_currentClipPlane{ glm::vec4(0.0f) };
    };
}
