// This file is part of the SgOgl package.
// 
// Filename: EntityFactory.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "EntityFactory.h"
#include "Core.h"
#include "ecs/component/Components.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::ecs::factory::EntityFactory::EntityFactory(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[EntityFactory::EntityFactory()] Null pointer.");
    Log::SG_OGL_CORE_LOG_DEBUG("[EntityFactory::EntityFactory()] Create EntityFactory.");
}

sg::ogl::ecs::factory::EntityFactory::~EntityFactory() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[EntityFactory::~EntityFactory()] Destruct EntityFactory.");
}

//-------------------------------------------------
// Factories
//-------------------------------------------------

/*
entt::entity sg::ogl::ecs::factory::EntityFactory::CreateTppCharacterEntity(
    const ThirdPersonCameraSharedPtr& t_thirdPersonCamera,
    const std::string& t_fullModelFilePath,
    const glm::vec3& t_playerPosition,
    const glm::vec3& t_playerRotation,
    const glm::vec3& t_playerScale,
    const TerrainSharedPtr& t_terrain
) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add third person camera component
    t_thirdPersonCamera->SetPlayerPosition(t_playerPosition);
    m_application->registry.assign<component::ThirdPersonCameraComponent>(entity, t_thirdPersonCamera);

    // add skeletal model component
    const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_FlipUVs };
    m_application->registry.assign<component::SkeletalModelComponent>(
        entity,
        m_application->GetModelManager().GetSkeletalModelByPath(t_fullModelFilePath, pFlags)
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_playerPosition),
        glm::vec3(t_playerRotation),
        glm::vec3(t_playerScale)
    );

    // add health component
    m_application->registry.assign<component::HealthComponent>(entity);

    if (!t_terrain)
    {
        SG_OGL_CORE_LOG_WARN("[EntityFactory::CreateTppCharacterEntity()] No terrain object exists.");
    }

    // add player component
    m_application->registry.assign<component::PlayerComponent>(entity, t_terrain);

    return entity;
}
*/
