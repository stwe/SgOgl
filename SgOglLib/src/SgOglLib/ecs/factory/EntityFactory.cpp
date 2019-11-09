// This file is part of the SgOgl package.
// 
// Filename: EntityFactory.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "EntityFactory.h"
#include "Application.h"
#include "Log.h"
#include "Core.h"
#include "ecs/component/ModelComponent.h"
#include "ecs/component/TransformComponent.h"
#include "ecs/component/MeshComponent.h"
#include "ecs/component/CubemapComponent.h"
#include "ecs/component/SkydomeComponent.h"
#include "ecs/component/TerrainComponent.h"
#include "ecs/component/GuiComponent.h"
#include "ecs/component/InstancesComponent.h"
#include "resource/Model.h"
#include "resource/ModelManager.h"
#include "resource/TextureManager.h"
#include "terrain/Terrain.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::ecs::factory::EntityFactory::EntityFactory(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[EntityFactory::EntityFactory()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[EntityFactory::EntityFactory()] Create EntityFactory.");
}

sg::ogl::ecs::factory::EntityFactory::~EntityFactory() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[EntityFactory::~EntityFactory()] Destruct EntityFactory.");
}

//-------------------------------------------------
// Factories
//-------------------------------------------------

void sg::ogl::ecs::factory::EntityFactory::CreateModelEntity(
    const std::string& t_fullModelFilePath,
    const glm::vec3& t_position,
    const glm::vec3& t_rotation,
    const glm::vec3& t_scale
) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add model component
    m_application->registry.assign<component::ModelComponent>(
        entity,
        m_application->GetModelManager().GetModelByPath(t_fullModelFilePath)
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_position.x, t_position.y, t_position.z),
        glm::vec3(t_rotation.x, t_rotation.y, t_rotation.z),
        glm::vec3(t_scale.x, t_scale.y, t_scale.z)
    );
}

void sg::ogl::ecs::factory::EntityFactory::CreateModelEntity(
    uint32_t t_instances,
    const std::string& t_fullModelFilePath,
    const std::vector<glm::mat4>& t_matrices
) const
{
    const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals };

    for (auto& mesh : m_application->GetModelManager().GetModelByPath(t_fullModelFilePath, pFlags)->GetMeshes())
    {
        // get Vao of the mesh
        auto& vao{ mesh->GetVao() };

        // create an empty Vbo for instanced data
        const uint32_t numberOfFloatsPerInstance{ 16 };
        const auto vbo{ vao.AddEmptyVbo(numberOfFloatsPerInstance * t_instances) };

        // set Vbo attributes
        vao.AddInstancedAttribute(vbo, 5, 4, numberOfFloatsPerInstance, 0);
        vao.AddInstancedAttribute(vbo, 6, 4, numberOfFloatsPerInstance, 4);
        vao.AddInstancedAttribute(vbo, 7, 4, numberOfFloatsPerInstance, 8);
        vao.AddInstancedAttribute(vbo, 8, 4, numberOfFloatsPerInstance, 12);

        // store data
        vao.StoreTransformationMatrices(vbo, numberOfFloatsPerInstance * t_instances, t_matrices);
    }

    // create an entity
    const auto entity{ m_application->registry.create() };

    // add instances component
    m_application->registry.assign<component::InstancesComponent>(
        entity,
        t_instances
    );

    // add model component
    m_application->registry.assign<component::ModelComponent>(
        entity,
        m_application->GetModelManager().GetModelByPath(t_fullModelFilePath)
    );
}

void sg::ogl::ecs::factory::EntityFactory::CreateSkyboxEntity(const std::vector<std::string>& t_cubemapFileNames) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add mesh component
    m_application->registry.assign<component::MeshComponent>(
        entity,
        m_application->GetModelManager().GetStaticMeshByName(resource::ModelManager::SKYBOX_MESH)
    );

    // add cubemap component
    m_application->registry.assign<sg::ogl::ecs::component::CubemapComponent>(
        entity,
        m_application->GetTextureManager().GetCubemapId(t_cubemapFileNames)
    );
}

void sg::ogl::ecs::factory::EntityFactory::CreateSkydomeEntity(const std::string& t_fullModelFilePath) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add model component
    m_application->registry.assign<component::ModelComponent>(
        entity,
        m_application->GetModelManager().GetModelByPath(t_fullModelFilePath)
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(m_application->GetProjectionOptions().farPlane * 0.5f)
    );

    // add skydome component/tag
    m_application->registry.assign<component::SkydomeComponent>(entity);
}

void sg::ogl::ecs::factory::EntityFactory::CreateTerrainEntity(const std::shared_ptr<terrain::Terrain>& t_terrain) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add terrain component
    m_application->registry.assign<component::TerrainComponent>(
        entity,
        t_terrain
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_terrain->GetTerrainOptions().xPos, 0.0f, t_terrain->GetTerrainOptions().zPos)
    );
}

void sg::ogl::ecs::factory::EntityFactory::CreateGuiEntity(
    const float t_x,
    const float t_y,
    const float t_scaleX,
    const float t_scaleY,
    const uint32_t t_textureId
) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_x, t_y, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(t_scaleX, t_scaleY, 1.0f)
    );

    // add mesh component
    m_application->registry.assign<component::MeshComponent>(
        entity,
        m_application->GetModelManager().GetStaticMeshByName(resource::ModelManager::GUI_MESH)
    );

    // add gui component
    m_application->registry.assign<component::GuiComponent>(entity, t_textureId);
}
