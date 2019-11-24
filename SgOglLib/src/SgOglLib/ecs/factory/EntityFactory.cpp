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
#include "buffer/Vbo.h"
#include "ecs/component/ModelComponent.h"
#include "ecs/component/TransformComponent.h"
#include "ecs/component/MeshComponent.h"
#include "ecs/component/CubemapComponent.h"
#include "ecs/component/SkydomeComponent.h"
#include "ecs/component/TerrainComponent.h"
#include "ecs/component/GuiComponent.h"
#include "ecs/component/InstancesComponent.h"
#include "ecs/component/WaterComponent.h"
#include "ecs/component/ParticleEmitterComponent.h"
#include "ecs/component/MoveableComponent.h"
#include "resource/Model.h"
#include "resource/ModelManager.h"
#include "resource/TextureManager.h"
#include "terrain/Terrain.h"
#include "water/Water.h"
#include "particle/ParticleEmitter.h"

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
    const glm::vec3& t_scale,
    const bool t_moveable,
    const bool t_showTriangles
) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add model component
    m_application->registry.assign<component::ModelComponent>(
        entity,
        m_application->GetModelManager().GetModelByPath(t_fullModelFilePath),
        t_showTriangles,
        false // todo fakeNormals
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_position.x, t_position.y, t_position.z),
        glm::vec3(t_rotation.x, t_rotation.y, t_rotation.z),
        glm::vec3(t_scale.x, t_scale.y, t_scale.z)
    );

    // add moveable component
    if (t_moveable)
    {
        m_application->registry.assign<component::MoveableComponent>(entity);
    }
}

void sg::ogl::ecs::factory::EntityFactory::CreateModelEntity(
    const uint32_t t_instances,
    const std::string& t_fullModelFilePath,
    const std::vector<glm::mat4>& t_matrices,
    bool t_fakeNormals
) const
{
    const uint32_t numberOfFloatsPerInstance{ 16 };

    // create an empty Vbo for instanced data
    const auto vbo{ buffer::Vbo::GenerateVbo() };

    // init empty
    // todo: remove?
    buffer::Vbo::InitEmpty(vbo, numberOfFloatsPerInstance * t_instances, GL_STATIC_DRAW);

    // store data
    buffer::Vbo::StoreTransformationMatrices(vbo, numberOfFloatsPerInstance * t_instances, t_matrices);

    // bind Vbo to each mesh
    // todo
    const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals };
    for (auto& mesh : m_application->GetModelManager().GetModelByPath(t_fullModelFilePath, pFlags)->GetMeshes())
    {
        // get Vao of the mesh
        auto& vao{ mesh->GetVao() };
        vao.BindVao();

        // set Vbo attributes
        buffer::Vbo::AddInstancedAttribute(vbo, 5, 4, numberOfFloatsPerInstance, 0);
        buffer::Vbo::AddInstancedAttribute(vbo, 6, 4, numberOfFloatsPerInstance, 4);
        buffer::Vbo::AddInstancedAttribute(vbo, 7, 4, numberOfFloatsPerInstance, 8);
        buffer::Vbo::AddInstancedAttribute(vbo, 8, 4, numberOfFloatsPerInstance, 12);

        buffer::Vao::UnbindVao();
    }
    SG_OGL_CORE_LOG_WARN("[EntityFactory::CreateModelEntity()] The Vao for the model {} has been changed.", t_fullModelFilePath);

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
        m_application->GetModelManager().GetModelByPath(t_fullModelFilePath, pFlags),
        false, // showTriangles - todo
        t_fakeNormals
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
    m_application->registry.assign<component::CubemapComponent>(
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

void sg::ogl::ecs::factory::EntityFactory::CreateWaterEntity(const std::shared_ptr<water::Water>& t_water) const
{
    // create an entity
    const auto entity{ m_application->registry.create() };

    // add mesh component
    m_application->registry.assign<component::MeshComponent>(
        entity,
        m_application->GetModelManager().GetStaticMeshByName(resource::ModelManager::WATER_MESH)
    );

    // add water component
    m_application->registry.assign<component::WaterComponent>(
        entity,
        t_water
    );

    // add transform component
    m_application->registry.assign<component::TransformComponent>(
        entity,
        glm::vec3(t_water->GetXPosition(), t_water->GetHeight(), t_water->GetZPosition()),
        glm::vec3(0.0f),
        t_water->GetTileSize()
    );
}

void sg::ogl::ecs::factory::EntityFactory::CreateParticleEntity(std::shared_ptr<particle::ParticleEmitter>& t_particleEmitter) const
{
    // create an empty Vbo for instanced data
    const auto vbo{ buffer::Vbo::GenerateVbo() };

    // init empty
    const auto floatCount{ particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE * static_cast<uint32_t>(t_particleEmitter->GetMaxParticles()) };
    buffer::Vbo::InitEmpty(vbo, floatCount, GL_STREAM_DRAW);

    // create Mesh
    auto meshSharedPtr{ std::make_shared<resource::Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(
        resource::ModelManager::GetParticleVertices().data(),
        static_cast<int32_t>(resource::ModelManager::GetParticleVertices().size()) / 2,
        bufferLayout
    );

    // add empty Vbo to the Mesh
    auto& vao{ meshSharedPtr->GetVao() };
    vao.BindVao();

    buffer::Vbo::AddInstancedAttribute(vbo, 1, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 0);
    buffer::Vbo::AddInstancedAttribute(vbo, 2, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 4);
    buffer::Vbo::AddInstancedAttribute(vbo, 3, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 8);
    buffer::Vbo::AddInstancedAttribute(vbo, 4, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 12);
    buffer::Vbo::AddInstancedAttribute(vbo, 5, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 16);
    buffer::Vbo::AddInstancedAttribute(vbo, 6, 4, particle::ParticleEmitter::NUMBER_OF_FLOATS_PER_INSTANCE, 20);

    buffer::Vao::UnbindVao();

    // store Vbo Id
    t_particleEmitter->SetVboId(vbo);

    // create an entity
    const auto entity{ m_application->registry.create() };

    // add particle emitter component
    m_application->registry.assign<component::ParticleEmitterComponent>(
        entity,
        t_particleEmitter
    );

    // add mesh component
    m_application->registry.assign<component::MeshComponent>(
        entity,
        meshSharedPtr
    );
}
