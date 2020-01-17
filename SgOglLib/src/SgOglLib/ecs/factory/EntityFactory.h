// This file is part of the SgOgl package.
// 
// Filename: EntityFactory.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
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
    class Terrain;
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
        using TerrainSharedPtr = std::shared_ptr<terrain::Terrain>;
        using WaterSharedPtr = std::shared_ptr<water::Water>;
        using ParticleEmitterSharedPtr = std::shared_ptr<particle::ParticleEmitter>;

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
         * @brief Creates an Entity from a model file.
         * @param t_fullModelFilePath The file path to the model.
         * @param t_position The position of the model.
         * @param t_rotation The rotation of the model.
         * @param t_scale The scale of the model.
         * @param t_showTriangles Render primitives as wireframes.
         * @return The created Entity.
         */
        entt::entity CreateModelEntity(
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation,
            const glm::vec3& t_scale,
            bool t_showTriangles = false
        ) const;

        /**
         * @brief Creates an Entity from a model file and add a PointLightComponent.
         * @param t_pointLight Point light which is added as component.
         * @param t_name The name of the Point Light.
         * @param t_fullModelFilePath The file path to the model.
         * @param t_position The position of the model.
         * @param t_rotation The rotation of the model.
         * @param t_scale The scale of the model.
         * @param t_showTriangles Render primitives as wireframes.
         * @return The created Entity.
         */
        entt::entity CreatePointLightEntity(
            const PointLightSharedPtr& t_pointLight,
            const std::string& t_name,
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation,
            const glm::vec3& t_scale,
            bool t_showTriangles = false
        ) const;

        /**
         * @brief Creates a Sun Entity.
         * @param t_sun Directional light (Sun) which is added as component.
         * @return The created Entity.
         */
        entt::entity CreateSunEntity(const SunSharedPtr& t_sun) const;

        /**
         * @brief Creates a Gui Entity.
         * @param t_posX The x position of the Gui.
         * @param t_posY The y position of the Gui.
         * @param t_scaleX The x scale value.
         * @param t_scaleY The y scale value.
         * @param t_textureId The id of the texture to be displayed.
         * @return The created Entity.
         */
        entt::entity CreateGuiEntity(
            float t_posX,
            float t_posY,
            float t_scaleX,
            float t_scaleY,
            uint32_t t_textureId
        ) const;

        /**
         * @brief Creates a Skybox Entity.
         * @param t_cubemapFileNames Full file paths to the Skybox texture files.
         * @return The created Entity.
         */
        entt::entity CreateSkyboxEntity(const std::vector<std::string>& t_cubemapFileNames) const;

        /**
         * @brief Creates a Skydome Entity.
         * @param t_fullModelFilePath Full file path to the Skydome model file.
         * @return The created Entity.
         */
        entt::entity CreateSkydomeEntity(const std::string& t_fullModelFilePath) const;

        /**
         * @brief Creates a Water Entity.
         * @param t_water A Water instance which is added as component.
         * @return The created Entity.
         */
        entt::entity CreateWaterEntity(const WaterSharedPtr& t_water) const;


        // todo

        void CreateTerrainQuadtreeEntity(const std::shared_ptr<terrain::TerrainQuadtree>& t_terrainQuadtree) const;


        ///////////////////////////////////////////////////////////////////////


        void CreateModelEntity(
            uint32_t t_instances,
            const std::string& t_fullModelFilePath,
            const std::vector<glm::mat4>& t_matrices,
            bool t_fakeNormals = false
        ) const;

        entt::entity CreateSkeletalModelEntity(
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation,
            const glm::vec3& t_scale,
            bool t_showTriangles = false,
            bool t_fakeNormals = false,
            bool t_useExistingNormalmaps = false,
            bool t_moveable = false
        ) const;

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
        entt::entity CreateTppCharacterEntity(
            const ThirdPersonCameraSharedPtr& t_thirdPersonCamera,
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_playerPosition,
            const glm::vec3& t_playerRotation,
            const glm::vec3& t_playerScale,
            const TerrainSharedPtr& t_terrain
        ) const;

        void CreateTerrainEntity(const TerrainSharedPtr& t_terrain) const;

        void CreateParticleEntity(ParticleEmitterSharedPtr& t_particleEmitter) const;

    protected:

    private:
        Application* m_application{ nullptr };
    };
}
