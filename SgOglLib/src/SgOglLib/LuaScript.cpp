// This file is part of the SgOgl package.
// 
// Filename: LuaScript.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include "LuaScript.h"
#include "Application.h"
#include "SgOglException.h"
#include "Core.h"
#include "camera/FirstPersonCamera.h"
#include "camera/ThirdPersonCamera.h"
#include "resource/ModelManager.h"
#include "resource/TextureManager.h"
#include "resource/SkeletalModel.h"
#include "input/MouseInput.h"
#include "water/Water.h"
#include "particle/ParticleSystem.h"
#include "terrain/TerrainConfig.h"
#include "terrain/TerrainQuadtree.h"
#include "ecs/system/ForwardRenderSystem.h"
#include "ecs/system/DeferredRenderSystem.h"
#include "ecs/system/SkeletalModelRenderSystem.h"
#include "ecs/system/SkydomeRenderSystem.h"
#include "ecs/system/SunRenderSystem.h"
#include "ecs/system/SkyboxRenderSystem.h"
#include "ecs/system/GuiRenderSystem.h"
#include "ecs/system/WaterRenderSystem.h"
#include "ecs/system/ParticleSystemRenderer.h"
#include "ecs/system/InstancingRenderSystem.h"
#include "ecs/system/TerrainQuadtreeRenderSystem.h"
#include "ecs/system/TerrainQuadtreeWfRenderSystem.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::LuaScript::LuaScript(Application* t_application, const std::string& t_fileName)
    : m_application{ t_application }
    , m_fileName{ t_fileName }
{
    SG_OGL_CORE_ASSERT(m_application, "[LuaScript::LuaScript()] Null pointer.");

    Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::LuaScript()] Create LuaScript.");

    LoadFile();
}

//-------------------------------------------------
// Config
//-------------------------------------------------

void sg::ogl::LuaScript::LoadFile()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::LoadFile()] Create content from {}.", m_fileName);

    InitLua();
    RunLuaScript();
    FinishLuaScript();
}

void sg::ogl::LuaScript::InitLua()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

    RegisterFunctions();
    CreateGlmUsertypes();
    CreateSceneUsertype();

    CreateRendererUsertype<ecs::system::ForwardRenderSystem>("ForwardRenderer");
    CreateRendererUsertype<ecs::system::DeferredRenderSystem>("DeferredRenderer");
    CreateRendererUsertype<ecs::system::SkeletalModelRenderSystem>("SkeletalModelRenderer");
    CreateRendererUsertype<ecs::system::SkydomeRenderSystem>("SkydomeRenderer");
    CreateRendererUsertype<ecs::system::SunRenderSystem>("SunRenderer");
    CreateRendererUsertype<ecs::system::SkyboxRenderSystem>("SkyboxRenderer");
    CreateRendererUsertype<ecs::system::GuiRenderSystem>("GuiRenderer");
    CreateRendererUsertype<ecs::system::WaterRenderSystem>("WaterRenderer");
    CreateRendererUsertype<ecs::system::ParticleSystemRenderer>("ParticleSystemRenderer");
    CreateRendererUsertype<ecs::system::TextRenderSystem>("TextRenderer");
    CreateRendererUsertype<ecs::system::InstancingRenderSystem>("InstancingRenderer");
    CreateRendererUsertype<ecs::system::TerrainQuadtreeRenderSystem>("TerrainQuadtreeRenderer");
    CreateRendererUsertype<ecs::system::TerrainQuadtreeWfRenderSystem>("TerrainQuadtreeWfRenderer");

    CreateCameraUsertypes();
    CreateResourceUsertypes();
    CreateComponentUsertypes();
    CreateEcsRegistryUsertype();
}

void sg::ogl::LuaScript::RunLuaScript()
{
    const auto result{ m_lua.script_file(m_fileName) };
    if (!result.valid())
    {
        throw SG_OGL_EXCEPTION("[LuaScript::RunLuaScript()] Unable to load and execute Lua-Script from file " + m_fileName + " .");
    }
}

void sg::ogl::LuaScript::FinishLuaScript() const
{
    SG_OGL_CORE_ASSERT(m_application->currentScene, "[LuaScript::FinishLuaScript()] Null pointer.");

    // sort renderer by priority
    std::sort(m_application->currentScene->renderer.begin(), m_application->currentScene->renderer.end(), [](const auto& t_lhs, const auto& t_rhs)
    {
        return t_lhs->priority > t_rhs->priority;
    });

    Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] ---------------------------");
    Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] Renderer priority settings.");

    auto i{ 0 };
    for (const auto& renderer : m_application->currentScene->renderer)
    {
        Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] {}. {}", i, renderer->name);
        i++;
    }

    Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] ---------------------------");
}

//-------------------------------------------------
// Usertypes
//-------------------------------------------------

void sg::ogl::LuaScript::RegisterFunctions()
{
    m_lua.new_usertype<ProjectionOptions>(
        "ProjectionOptions",
        sol::no_constructor,
        "width", &ProjectionOptions::width,
        "height", &ProjectionOptions::height
    );

    m_lua.new_usertype<Application>(
        "Application",
        sol::no_constructor,
        "GetProjectionOptions", [](Application& t_application)
        {
            return t_application.GetProjectionOptions();
        }
    );

    m_lua.new_usertype<input::MouseInput>(
        "MouseInput",
        sol::no_constructor,
        "IsLeftButtonPressed", &input::MouseInput::IsLeftButtonPressed,
        "IsRightButtonPressed", &input::MouseInput::IsRightButtonPressed,
        "GetCurrentPos", &input::MouseInput::GetCurrentPos
    );

    m_lua["applicationContext"] = m_application;
    m_lua["modelManager"] = &m_application->GetModelManager();
    m_lua["textureManager"] = &m_application->GetTextureManager();
    m_lua["mouseInput"] = &m_application->GetMouseInput();
    m_lua["ecs"] = &m_application->registry;
}

void sg::ogl::LuaScript::CreateGlmUsertypes()
{
    // glm::ivec2
    m_lua.new_usertype<glm::ivec2>(
        "Ivec2",
        sol::constructors<glm::ivec2(), glm::ivec2(int), glm::ivec2(int, int)>(),
        "x", &glm::ivec2::x,
        "y", &glm::ivec2::y
    );

    // glm::vec2
    m_lua.new_usertype<glm::vec2>(
        "Vec2",
        sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
        "x", &glm::vec2::x,
        "y", &glm::vec2::y,
        sol::meta_function::addition, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator/)
    );

    // glm::vec3
    m_lua.new_usertype<glm::vec3>(
        "Vec3",
        sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z,
        sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator/)
    );

    // glm::vec4
    m_lua.new_usertype<glm::vec4>(
        "Vec4",
        sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::w,
        sol::meta_function::addition, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec4(const glm::vec4&, const glm::vec4&)>(glm::operator/)
    );
}

void sg::ogl::LuaScript::CreateSceneUsertype()
{
    m_lua.new_usertype<scene::Scene>(
        "Scene",
        sol::constructors<scene::Scene(Application*)>(),
        "new", sol::factories(
            [&](Application* t_app)-> std::shared_ptr<scene::Scene>
            {
                auto scene{ std::make_unique<scene::Scene>(t_app) };
                t_app->currentScene = scene.get();
                scene->SetParentLuaState(m_lua);
                return scene;
            }
        ),
        "SetAmbientIntensity", &scene::Scene::SetAmbientIntensity,
        "SetCurrentCamera", &scene::Scene::SetCurrentCameraByName
    );
}

void sg::ogl::LuaScript::CreateCameraUsertypes()
{
    // FirstPersonCamera
    m_lua.new_usertype<camera::FirstPersonCamera>(
        "FirstPersonCamera",
        sol::constructors<
            camera::FirstPersonCamera(const std::string&, Application*),
            camera::FirstPersonCamera(const std::string&, Application*, const glm::vec3&, float, float)
        >(),
        "new", sol::factories(
            [](const std::string& t_name, Application* t_app, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateCameraUsertypes()] Add {} FirstPersonCamera to the current Scene.", t_name);
                t_currentScene->cameras.emplace(t_name, std::make_unique<camera::FirstPersonCamera>(t_name, t_app));
                return dynamic_cast<camera::FirstPersonCamera*>(t_currentScene->cameras.at(t_name).get());
            },
            [](const std::string& t_name, Application* t_app, const glm::vec3& t_pos, float t_yaw, float t_pitch, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateCameraUsertypes()] Add {} FirstPersonCamera to the current Scene.", t_name);
                t_currentScene->cameras.emplace(t_name, std::make_unique<camera::FirstPersonCamera>(t_name, t_app, t_pos, t_yaw, t_pitch));
                return dynamic_cast<camera::FirstPersonCamera*>(t_currentScene->cameras.at(t_name).get());
            }
        ),
        "SetCameraVelocity", &camera::FirstPersonCamera::SetCameraVelocity,
        "SetMouseSensitivity", &camera::FirstPersonCamera::SetMouseSensitivity,
        "GetCameraVelocity", &camera::FirstPersonCamera::GetCameraVelocity,
        "GetMouseSensitivity", &camera::FirstPersonCamera::GetMouseSensitivity
    );

    // ThirdPersonCamera
    m_lua.new_usertype<camera::ThirdPersonCamera>(
        "ThirdPersonCamera",
        sol::constructors<
            camera::ThirdPersonCamera(const std::string&, Application*, const glm::vec3&)
        >(),
        "new", sol::factories(
            [](const std::string& t_name, Application* t_app, const glm::vec3& t_pos, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateCameraUsertypes()] Add {} ThirdPersonCamera to the current Scene.", t_name);
                t_currentScene->cameras.emplace(t_name, std::make_unique<camera::ThirdPersonCamera>(t_name, t_app, t_pos));
                return dynamic_cast<camera::ThirdPersonCamera*>(t_currentScene->cameras.at(t_name).get());
            }
        ),
        "SetPlayerPosition", &camera::ThirdPersonCamera::SetPlayerPosition,
        "SetPlayerRotationY", &camera::ThirdPersonCamera::SetPlayerRotationY,
        "GetPlayerRotationY", &camera::ThirdPersonCamera::GetPlayerRotationY,
        "SetYaw", &camera::ThirdPersonCamera::SetYaw,
        "SetPitch", &camera::ThirdPersonCamera::SetPitch
    );
}

void sg::ogl::LuaScript::CreateResourceUsertypes()
{
    // ModelManager
    m_lua.new_usertype<resource::ModelManager>(
        "ModelManager",
        sol::no_constructor,
        "GetMaterialByName", &resource::ModelManager::GetMaterialByName,
        "GetModel", &resource::ModelManager::GetModel,
        "GetSkeletalModel", &resource::ModelManager::GetSkeletalModel
    );

    // TextureManager
    m_lua.new_usertype<resource::TextureManager>(
        "TextureManager",
        sol::no_constructor,
        "LoadTexture", &resource::TextureManager::LoadTexture,
        "LoadTextureVerticalFlipped", &resource::TextureManager::LoadTextureVerticalFlipped,
        "GetCubemapId", [](resource::TextureManager& t_textureManager, sol::as_table_t<std::vector<std::string>> t_table)
        {
            return t_textureManager.GetCubemapId(t_table.value());
        }
    );

    // Water
    m_lua.new_usertype<water::Water>(
        "Water",
        sol::constructors<water::Water(Application*, float ,float, float, const glm::vec3&, const std::string&, const std::string&)>(),
        "new", sol::factories(
            [](const std::string& t_name,
                Application* t_app,
                float t_xPos, float t_zPos,
                float t_height,
                const glm::vec3& t_tileSize,
                const std::string& t_dudvMapFilePath, const std::string& t_normalMapFilePath,
                scene::Scene* t_currentScene
            )
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateResourceUsertypes()] Add {} Water to the current Scene.", t_name);
                t_currentScene->waterSurfaces.emplace(
                    t_name,
                    std::make_unique<water::Water>(
                        t_app, t_xPos, t_zPos, t_height, t_tileSize, t_dudvMapFilePath, t_normalMapFilePath
                    )
                );
                return t_currentScene->waterSurfaces.at(t_name).get();
            }
        ),
        "AddRendererToReflectionTexture", &water::Water::AddRendererToReflectionTexture,
        "AddRendererToRefractionTexture", &water::Water::AddRendererToRefractionTexture
    );

    // ParticleSystem
    m_lua.new_usertype<particle::ParticleSystem>(
        "ParticleSystem",
        sol::constructors<
            particle::ParticleSystem(scene::Scene*, uint32_t, int),
            particle::ParticleSystem(scene::Scene*, uint32_t, int, float, float, float, float, float)
        >(),
        "new", sol::factories(
            [](const std::string& t_name, uint32_t t_textureId, int t_textureRows, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateResourceUsertypes()] Add {} ParticleSystem to the current Scene.", t_name);
                t_currentScene->particleSystems.emplace(t_name, std::make_unique<particle::ParticleSystem>(t_currentScene, t_textureId, t_textureRows));
                return t_currentScene->particleSystems.at(t_name).get();
            },
            [](const std::string& t_name, uint32_t t_textureId, int t_textureRows, float t_particlesPerSecond, float t_speed, float t_gravityEffect, float t_lifeTime, float t_maxScale, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateResourceUsertypes()] Add {} ParticleSystem to the current Scene.", t_name);
                t_currentScene->particleSystems.emplace(t_name, std::make_unique<particle::ParticleSystem>(t_currentScene, t_textureId, t_textureRows, t_particlesPerSecond, t_speed, t_gravityEffect, t_lifeTime, t_maxScale));
                return t_currentScene->particleSystems.at(t_name).get();
            }
        ),
        "SetParticlesPerSecond", &particle::ParticleSystem::SetParticlesPerSecond,
        "SetSpeed", &particle::ParticleSystem::SetSpeed,
        "SetGravityEffect", &particle::ParticleSystem::SetGravityEffect,
        "SetLifeTime", &particle::ParticleSystem::SetLifeTime,
        "SetMaxScale", &particle::ParticleSystem::SetMaxScale,
        "GenerateParticles", &particle::ParticleSystem::GenerateParticles,
        "instancing", &particle::ParticleSystem::instancing
    );

    m_lua.new_usertype<terrain::TerrainConfig>(
        "TerrainConfig",
        sol::constructors<
            terrain::TerrainConfig(Application*)
        >(),
        "new", sol::factories(
            [](Application* t_app, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateResourceUsertypes()] Add a TerrainConfig to the current Scene.");
                t_currentScene->terrainConfig = std::make_unique<terrain::TerrainConfig>(t_app);
                return t_currentScene->terrainConfig;
            }
        ),
        "InitMapsAndMorphing", &terrain::TerrainConfig::InitMapsAndMorphing,
        "InitTextures", &terrain::TerrainConfig::InitTextures,
        "GetHeightAt", &terrain::TerrainConfig::GetHeightAt,
        "scaleXz", &terrain::TerrainConfig::scaleXz,
        "scaleY", &terrain::TerrainConfig::scaleY,
        "rootNodes", &terrain::TerrainConfig::rootNodes,
        "normalStrength", &terrain::TerrainConfig::normalStrength,
        "lodRanges", &terrain::TerrainConfig::lodRanges,
        "use16BitHeightmap", &terrain::TerrainConfig::use16BitHeightmap
    );

    m_lua.new_usertype<terrain::TerrainQuadtree>(
        "TerrainQuadtree",
        sol::constructors<
            terrain::TerrainQuadtree(scene::Scene*, const std::shared_ptr<terrain::TerrainConfig>&)
        >(),
        "new", sol::factories(
            [](scene::Scene* t_currentScene, const std::shared_ptr<terrain::TerrainConfig>& t_terrainConfig)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateResourceUsertypes()] Add a Terrain to the current Scene.");
                t_currentScene->terrain = std::make_unique<terrain::TerrainQuadtree>(t_currentScene, t_terrainConfig);
                return t_currentScene->terrain;
            }
        )
    );
}

void sg::ogl::LuaScript::CreateComponentUsertypes()
{
    m_lua.new_usertype<ecs::component::ModelComponent>(
        "ModelComponent"
    );

    m_lua.new_usertype<ecs::component::ModelInstancesComponent>(
        "ModelInstancesComponent"
    );

    m_lua.new_usertype<ecs::component::SkeletalModelComponent>(
        "SkeletalModelComponent"
    );

    m_lua.new_usertype<math::Transform>(
        "Transform",
        "position", &math::Transform::position,
        "rotation", &math::Transform::rotation,
        "scale", &math::Transform::scale
    );

    m_lua.new_usertype<resource::Material>(
        "Material"
    );

    m_lua.new_usertype<ecs::component::SkydomeComponent>(
        "SkydomeComponent"
    );

    m_lua.new_usertype<light::PointLight>(
        "PointLight",
        "position", &light::PointLight::position
    );

    m_lua.new_usertype<light::DirectionalLight>(
        "DirectionalLight"
    );

    m_lua.new_usertype<light::Sun>(
        "Sun"
    );

    m_lua.new_usertype<ecs::component::UpdateComponent>(
        "UpdateComponent"
    );

    m_lua.new_usertype<ecs::component::InputComponent>(
        "InputComponent"
    );

    m_lua.new_usertype<ecs::component::CubemapComponent>(
        "CubemapComponent"
    );

    m_lua.new_usertype<ecs::component::GuiComponent>(
        "GuiComponent"
    );

    m_lua.new_usertype<ecs::component::WaterComponent>(
        "WaterComponent"
    );

    m_lua.new_usertype<ecs::component::ParticleSystemComponent>(
        "ParticleSystemComponent"
    );

    m_lua.new_usertype<ecs::component::TextComponent>(
        "TextComponent"
    );

    m_lua.new_usertype<ecs::component::TerrainQuadtreeComponent>(
        "TerrainQuadtreeComponent"
    );

    m_lua.new_usertype<ecs::component::PlayerComponent>(
        "PlayerComponent"
    );
}

void sg::ogl::LuaScript::CreateEcsRegistryUsertype()
{
    // EnTT registry
    m_lua.new_usertype<entt::registry>(
        "Registry",
        sol::no_constructor,
        "CreateEntity", static_cast<entt::entity(entt::registry::*)()>(&entt::registry::create),
        "AddModelComponent", static_cast<ecs::component::ModelComponent& (entt::registry::*)(entt::entity, std::shared_ptr<resource::Model>&, bool&&)>(&entt::registry::emplace<ecs::component::ModelComponent, std::shared_ptr<resource::Model>&, bool>),
        "AddSkeletalModelComponent", static_cast<ecs::component::SkeletalModelComponent& (entt::registry::*)(entt::entity, std::shared_ptr<resource::SkeletalModel>&, bool&&)>(&entt::registry::emplace<ecs::component::SkeletalModelComponent, std::shared_ptr<resource::SkeletalModel>&, bool>),
        "AddTransformComponent", static_cast<math::Transform& (entt::registry::*)(entt::entity, glm::vec3&, glm::vec3&, glm::vec3&)>(&entt::registry::emplace<math::Transform, glm::vec3&, glm::vec3&, glm::vec3&>),
        "AddMaterialComponent", static_cast<resource::Material& (entt::registry::*)(entt::entity, resource::Material&)>(&entt::registry::emplace<resource::Material, resource::Material&>),
        "AddSkydomeComponent", static_cast<ecs::component::SkydomeComponent& (entt::registry::*)(entt::entity, std::string&&)>(&entt::registry::emplace<ecs::component::SkydomeComponent, std::string>),
        "AddPointLightComponent", static_cast<light::PointLight& (entt::registry::*)(entt::entity, glm::vec3&, glm::vec3&, glm::vec3&, glm::vec3&, float&&, float&&, float&&)>(&entt::registry::emplace<
            light::PointLight, glm::vec3&, glm::vec3&, glm::vec3&, glm::vec3&, float, float, float
        >),
        "AddDirectionalLightComponent", static_cast<light::DirectionalLight& (entt::registry::*)(entt::entity, glm::vec3&, glm::vec3&, glm::vec3&)>(&entt::registry::emplace<light::DirectionalLight, glm::vec3&, glm::vec3&, glm::vec3&>),
        "AddSunComponent", static_cast<light::Sun& (entt::registry::*)(entt::entity, glm::vec3&, glm::vec3&, glm::vec3&, uint32_t&&, float&&)>(&entt::registry::emplace<light::Sun, glm::vec3&, glm::vec3&, glm::vec3&, uint32_t, float>),
        "AddUpdateComponent", static_cast<ecs::component::UpdateComponent& (entt::registry::*)(entt::entity, std::string&&)>(&entt::registry::emplace<ecs::component::UpdateComponent, std::string>),
        "AddInputComponent", static_cast<ecs::component::InputComponent& (entt::registry::*)(entt::entity, std::string&&)>(&entt::registry::emplace<ecs::component::InputComponent, std::string>),
        "AddCubemapComponent", static_cast<ecs::component::CubemapComponent& (entt::registry::*)(entt::entity, uint32_t&&)>(&entt::registry::emplace<ecs::component::CubemapComponent, uint32_t>),
        "AddGuiComponent", static_cast<ecs::component::GuiComponent& (entt::registry::*)(entt::entity, uint32_t&&)>(&entt::registry::emplace<ecs::component::GuiComponent, uint32_t>),
        "AddWaterComponent", static_cast<ecs::component::WaterComponent& (entt::registry::*)(entt::entity, water::Water*&&)>(&entt::registry::emplace<ecs::component::WaterComponent, water::Water*>),
        "AddParticleSystemComponent", static_cast<ecs::component::ParticleSystemComponent& (entt::registry::*)(entt::entity, particle::ParticleSystem*&&)>(&entt::registry::emplace<ecs::component::ParticleSystemComponent, particle::ParticleSystem*>),
        "AddTextComponent", static_cast<ecs::component::TextComponent& (entt::registry::*)(entt::entity, std::string&&, float&&, float&&, float&&, glm::vec3&)>(&entt::registry::emplace<ecs::component::TextComponent, std::string, float, float, float, glm::vec3&>),
        "AddModelInstancesComponent",
        [](entt::registry& t_reg, entt::entity t_entity, std::shared_ptr<resource::Model>& t_model, bool t_showTriangles, bool t_fakeNormals, const sol::as_table_t<std::vector<math::Transform>>& t_transforms)
        {
            t_model->AddTransformVbo(t_transforms.value());
            auto instances{ static_cast<uint32_t>(t_transforms.value().size()) };
            t_reg.emplace<ecs::component::ModelInstancesComponent>(t_entity, t_model, t_showTriangles, t_fakeNormals, instances);
        },
        "AddTerrainQuadtreeComponent", static_cast<ecs::component::TerrainQuadtreeComponent& (entt::registry::*)(entt::entity, terrain::TerrainQuadtree*&&)>(&entt::registry::emplace<ecs::component::TerrainQuadtreeComponent, terrain::TerrainQuadtree*>),
        "AddPlayerComponent", static_cast<ecs::component::PlayerComponent& (entt::registry::*)(entt::entity, std::string&&, uint32_t&&, float&&, float&&)>(&entt::registry::emplace<ecs::component::PlayerComponent, std::string, uint32_t, float, float>),
        "GetPointLightComponent", static_cast<light::PointLight& (entt::registry::*)(entt::entity)>(&entt::registry::get<light::PointLight>)
    );
}
