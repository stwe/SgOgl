// This file is part of the SgOgl package.
// 
// Filename: LuaScript.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/vec3.hpp>
#include "LuaScript.h"
#include "Application.h"
#include "Core.h"
#include "SgOglException.h"
#include "camera/FirstPersonCamera.h"
#include "camera/ThirdPersonCamera.h"
#include "resource/ModelManager.h"
#include "ecs/system/ForwardRenderSystem.h"

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
    CreateRendererUsertypes();
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
        Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] {}. {}", i, renderer->debugName);
        i++;
    }

    Log::SG_OGL_CORE_LOG_INFO("[LuaScript::FinishLuaScript()] ---------------------------");
}

//-------------------------------------------------
// Usertypes
//-------------------------------------------------

void sg::ogl::LuaScript::RegisterFunctions()
{
    m_lua["applicationContext"] = m_application;
    m_lua["modelManager"] = &m_application->GetModelManager();
    m_lua["ecs"] = &m_application->registry;
}

void sg::ogl::LuaScript::CreateGlmUsertypes()
{
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
}

void sg::ogl::LuaScript::CreateSceneUsertype()
{
    m_lua.new_usertype<scene::Scene>(
        "Scene",
        sol::constructors<scene::Scene(Application*)>(),
        "new", sol::factories(
            [](Application* t_application)-> std::shared_ptr<scene::Scene>
            {
                auto scene{ std::make_unique<scene::Scene>(t_application) };
                t_application->currentScene = scene.get();
                return scene;
            }
        ),
        "SetAmbientIntensity", &scene::Scene::SetAmbientIntensity,
        "SetCurrentCamera", &scene::Scene::SetCurrentCameraByName
    );
}

void sg::ogl::LuaScript::CreateRendererUsertypes()
{
    // Forward renderer
    m_lua.new_usertype<ecs::system::ForwardRenderSystem>(
        "ForwardRenderer",
        sol::constructors<
        ecs::system::ForwardRenderSystem(scene::Scene*),
        ecs::system::ForwardRenderSystem(int t_priority, scene::Scene*)
        >(),
        "new", sol::factories(
            [](scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateRendererUsertypes()] Add ForwardRenderer to the current Scene.");
                t_currentScene->renderer.push_back(std::make_unique<ecs::system::ForwardRenderSystem>(t_currentScene));
            },
            [](int t_priority, scene::Scene* t_currentScene)
            {
                Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateRendererUsertypes()] Add ForwardRenderer to the current Scene.");
                t_currentScene->renderer.push_back(std::make_unique<ecs::system::ForwardRenderSystem>(t_priority, t_currentScene));
            }
        )
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
        "GetPlayerRotationY", &camera::ThirdPersonCamera::GetPlayerRotationY
    );
}

void sg::ogl::LuaScript::CreateResourceUsertypes()
{
    // ModelManager
    m_lua.new_usertype<resource::ModelManager>(
        "ModelManager",
        sol::no_constructor,
        "GetMaterialByName", &resource::ModelManager::GetMaterialByName,
        "GetModel", &resource::ModelManager::GetModel
    );
}

void sg::ogl::LuaScript::CreateComponentUsertypes()
{
    // Model component
    m_lua.new_usertype<ecs::component::ModelComponent>(
        "ModelComponent"
    );

    // Transform component
    m_lua.new_usertype<math::Transform>(
        "TransformComponent"
    );

    // Material component
    m_lua.new_usertype<resource::Material>(
        "MaterialComponent"
    );
}

void sg::ogl::LuaScript::CreateEcsRegistryUsertype()
{
    // EnTT registry
    m_lua.new_usertype<entt::registry>(
        "Registry",
        sol::no_constructor,
        "CreateEntity", static_cast<entt::entity(entt::registry::*)()>(&entt::registry::create),
        "AddModelComponent", &entt::registry::emplace<ecs::component::ModelComponent, std::shared_ptr<resource::Model>&, bool>,
        "AddTransformComponent", &entt::registry::emplace<math::Transform, glm::vec3&, glm::vec3&, glm::vec3&>,
        "AddMaterialComponent", &entt::registry::emplace<std::shared_ptr<resource::Material>>
    );
}
