// This file is part of the SgOgl package.
// 
// Filename: EntityFactory.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>
#include "Application.h"

namespace sg::ogl::camera
{
    class ThirdPersonCamera;
}

namespace sg::ogl::light
{
    struct PointLight;
    struct Sun;
}

namespace sg::ogl::terrain
{
    class TerrainQuadtree;
}

namespace sg::ogl::water
{
    class Water;
}

namespace sg::ogl::particle
{
    class ParticleEmitter;
}

namespace sg::ogl::resource
{
    class SkeletalModel;
}

namespace sg::ogl::ecs::factory
{
    class EntityFactory
    {
    public:
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;
        using SunSharedPtr = std::shared_ptr<light::Sun>;
        using ThirdPersonCameraSharedPtr = std::shared_ptr<camera::ThirdPersonCamera>;
        using WaterSharedPtr = std::shared_ptr<water::Water>;
        using ParticleEmitterSharedPtr = std::shared_ptr<particle::ParticleEmitter>;
        using TerrainQuadtreeSharedPtr = std::shared_ptr<terrain::TerrainQuadtree>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        EntityFactory() = delete;

        explicit EntityFactory(Application* t_application);

        EntityFactory(const EntityFactory& t_other) = delete;
        EntityFactory(EntityFactory&& t_other) noexcept = delete;
        EntityFactory& operator=(const EntityFactory& t_other) = delete;
        EntityFactory& operator=(EntityFactory&& t_other) noexcept = delete;

        ~EntityFactory() noexcept;

        //-------------------------------------------------
        // Factories
        //-------------------------------------------------

        /**
         * @brief Creates a Terrain Quadtree Entity.
         * @param t_terrainQuadtree A Terrain Quadtree instance.
         */
        void CreateTerrainQuadtreeEntity(const TerrainQuadtreeSharedPtr& t_terrainQuadtree) const;

        /**
         * @brief Creates a character from a SkeletalModel file in the third person perspective.
         * @param t_thirdPersonCamera The camera that follows the character.
         * @param t_fullModelFilePath The file path to the skeletal model.
         * @param t_playerPosition The current position of the character.
         * @param t_playerRotation The current rotation of the character.
         * @param t_playerScale The scale of the character.
         * @param t_terrain A pointer to a terrain to get the current height position of the player.
         * @return The created Entity.
         */

        /*
        entt::entity CreateTppCharacterEntity(
            const ThirdPersonCameraSharedPtr& t_thirdPersonCamera,
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_playerPosition,
            const glm::vec3& t_playerRotation,
            const glm::vec3& t_playerScale,
            const TerrainSharedPtr& t_terrain
        ) const;
        */

    protected:

    private:
        Application* m_application{ nullptr };
    };
}
