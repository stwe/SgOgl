// This file is part of the SgOgl package.
// 
// Filename: Components.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::camera
{
    class ThirdPersonCamera;
}

namespace sg::ogl::light
{
    struct DirectionalLight;
}

namespace sg::ogl::resource
{
    class Mesh;
    class Model;
    struct Material;
    class SkeletalModel;
}

namespace sg::ogl::particle
{
    class ParticleEmitter;
}

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::water
{
    class Water;
}

namespace sg::ogl::ecs::component
{
    //-------------------------------------------------
    // Camera
    //-------------------------------------------------

    struct ThirdPersonCameraComponent
    {
        std::shared_ptr<camera::ThirdPersonCamera> thirdPersonCamera;
    };

    //-------------------------------------------------
    // Lighting
    //-------------------------------------------------

    struct DirectionalLightComponent
    {
        std::shared_ptr<light::DirectionalLight> directionalLight;
    };

    //-------------------------------------------------
    // Player
    //-------------------------------------------------

    struct HealthComponent
    {
        uint32_t health{ 0 };
    };

    struct PlayerComponent
    {
        static constexpr auto RUN_SPEED{ 40.0f };
        static constexpr auto TURN_SPEED{ 160.0f };
        static constexpr auto GRAVITY{ -50.0f };
        static constexpr auto JUMP_POWER{ 18.0f };

        float currentSpeed{ 0.0f };
        float currentTurnSpeed{ 0.0f };
        float upSpeed{ 0.0f };

        bool isInAir{ false };
    };

    //-------------------------------------------------
    // Texture / Material
    //-------------------------------------------------

    struct CubemapComponent
    {
        uint32_t cubemapId;
    };

    struct GuiComponent
    {
        uint32_t textureId{ 0 };
    };

    struct MaterialComponent
    {
        std::shared_ptr<resource::Material> material;
    };

    //-------------------------------------------------
    // Mesh / Model
    //-------------------------------------------------

    struct MeshComponent
    {
        std::shared_ptr<resource::Mesh> mesh;
    };

    struct ModelComponent
    {
        std::shared_ptr<resource::Model> model;
        bool showTriangles{ false };
        bool fakeNormals{ false };
        bool useExistingNormalmaps{ false };
    };

    struct SkeletalModelComponent
    {
        std::shared_ptr<resource::SkeletalModel> model;
        bool showTriangles{ false };
        bool fakeNormals{ false };
        bool useExistingNormalmaps{ false };
    };

    struct ParticleEmitterComponent
    {
        std::shared_ptr<particle::ParticleEmitter> particleEmitter;
    };

    struct InstancesComponent
    {
        uint32_t instances{ 0 };
    };

    struct MoveableComponent {};

    struct TransformComponent
    {
        glm::vec3 position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 rotation{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale{ glm::vec3(1.0f, 1.0f, 1.0f) };

        explicit operator glm::mat4() const
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = translate(modelMatrix, position);
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, scale);

            return modelMatrix;
        }
    };

    //-------------------------------------------------
    // Environment
    //-------------------------------------------------

    struct SkydomeComponent {};

    struct TerrainComponent
    {
        std::shared_ptr<terrain::Terrain> terrain;
    };

    struct WaterComponent
    {
        std::shared_ptr<water::Water> water;
    };
}
