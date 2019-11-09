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

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::ecs::factory
{
    class EntityFactory
    {
    public:
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

        void CreateModelEntity(
            const std::string& t_fullModelFilePath,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation,
            const glm::vec3& t_scale
        ) const;

        void CreateModelEntity(
            uint32_t t_instances,
            const std::string& t_fullModelFilePath,
            const std::vector<glm::mat4>& t_matrices
        ) const;

        void CreateSkyboxEntity(const std::vector<std::string>& t_cubemapFileNames) const;
        void CreateSkydomeEntity(const std::string& t_fullModelFilePath) const;
        void CreateTerrainEntity(const std::shared_ptr<terrain::Terrain>& t_terrain) const;

        void CreateGuiEntity(
            float t_x,
            float t_y,
            float t_scaleX,
            float t_scaleY,
            uint32_t t_textureId
        ) const;

    protected:

    private:
        Application* m_application{ nullptr };
    };
}
