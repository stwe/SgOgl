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
    class TerrainQuadtree;
}

namespace sg::ogl::water
{
    class Water;
}

namespace sg::ogl::ecs::component
{
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

        // todo terrain to get the current height

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
        uint32_t cubemapId{ 0 };
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

    //-------------------------------------------------
    // Environment
    //-------------------------------------------------

    struct SkydomeComponent {};

    struct TerrainQuadtreeComponent
    {
        std::shared_ptr<terrain::TerrainQuadtree> terrainQuadtree;
    };

    struct WaterComponent
    {
        std::shared_ptr<water::Water> water;
    };
}
