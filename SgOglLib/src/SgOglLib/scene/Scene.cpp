// This file is part of the SgOgl package.
// 
// Filename: Scene.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <assimp/postprocess.h>
#include "Scene.h"
#include "Core.h"
#include "Application.h"
#include "math/Transform.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "light/Sun.h"
#include "resource/ModelManager.h"
#include "resource/Material.h"
#include "camera/FirstPersonCamera.h"
#include "camera/ThirdPersonCamera.h"
#include "water/Water.h"
#include "ecs/component/Components.h"
#include "ecs/system/ForwardRenderSystem.h"
#include "ecs/system/DeferredRenderSystem.h"
#include "ecs/system/SkyboxRenderSystem.h"
#include "ecs/system/SkydomeRenderSystem.h"
#include "ecs/system/SunRenderSystem.h"
#include "ecs/system/GuiRenderSystem.h"
#include "ecs/system/WaterRenderSystem.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[Scene::Scene()] Null pointer.");

    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::Scene()] Create Scene.");
}

sg::ogl::scene::Scene::Scene(Application* t_application, const std::string& t_configFileName)
    : m_application{ t_application }
    , m_configFileName{ t_configFileName }
{
    SG_OGL_CORE_ASSERT(m_application, "[Scene::Scene()] Null pointer.");

    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::Scene()] Create Scene.");

    ConfigSceneFromFile();
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::~Scene()] Destruct Scene.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::Application* sg::ogl::scene::Scene::GetApplicationContext() const
{
    return m_application;
}

sg::ogl::camera::Camera& sg::ogl::scene::Scene::GetCurrentCamera() noexcept
{
    SG_OGL_CORE_ASSERT(m_currentCamera, "[Scene::GetCurrentCamera()] Null pointer.");
    return *m_currentCamera;
}

const sg::ogl::camera::Camera& sg::ogl::scene::Scene::GetCurrentCamera() const noexcept
{
    SG_OGL_CORE_ASSERT(m_currentCamera, "[Scene::GetCurrentCamera()] Null pointer.");
    return *m_currentCamera;
}

sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetCurrentDirectionalLight() noexcept
{
    SG_OGL_CORE_ASSERT(m_currentDirectionalLight, "[Scene::GetCurrentDirectionalLight()] Null pointer.");
    return *m_currentDirectionalLight;
}

const sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetCurrentDirectionalLight() const noexcept
{
    SG_OGL_CORE_ASSERT(m_currentDirectionalLight, "[Scene::GetCurrentDirectionalLight()] Null pointer.");
    return *m_currentDirectionalLight;
}

bool sg::ogl::scene::Scene::HasDirectionalLight() const
{
    return m_currentDirectionalLight != nullptr;
}

glm::vec4 sg::ogl::scene::Scene::GetCurrentClipPlane() const
{
    return m_currentClipPlane;
}

glm::vec3 sg::ogl::scene::Scene::GetAmbientIntensity() const
{
    return m_ambientIntensity;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Scene::SetAmbientIntensity(const glm::vec3& t_ambientIntensity)
{
    m_ambientIntensity = t_ambientIntensity;
}

void sg::ogl::scene::Scene::SetFirstPersonCameraAsCurrent(const FirstPersonCameraSharedPtr& t_fpCamera)
{
    m_currentCamera.reset();
    m_currentCamera = t_fpCamera;
}

void sg::ogl::scene::Scene::SetThirdPersonCameraAsCurrent(const ThirdPersonCameraSharedPtr& t_tpCamera)
{
    m_currentCamera.reset();
    m_currentCamera = t_tpCamera;
}

void sg::ogl::scene::Scene::SetCurrentCamera(const CameraSharedPtr& t_camera)
{
    m_currentCamera.reset();
    m_currentCamera = t_camera;
}

void sg::ogl::scene::Scene::SetCurrentDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight)
{
    m_currentDirectionalLight.reset();
    m_currentDirectionalLight = t_directionalLight;
}

void sg::ogl::scene::Scene::SetCurrentClipPlane(const glm::vec4& t_currentClipPlane)
{
    m_currentClipPlane = t_currentClipPlane;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Scene::Input()
{
    GetCurrentCamera().Input();
}

void sg::ogl::scene::Scene::Update(const double t_dt)
{
    GetCurrentCamera().Update(t_dt);

    for (auto& renderer : m_rendererArray)
    {
        renderer->Update(t_dt);
    }
}

void sg::ogl::scene::Scene::Render()
{
    auto handle{ SceneCache::rendererCache.load<RenderSystemLoader<ecs::system::WaterRenderSystem>>("WaterRenderSystem"_hs,this) };
    if (handle)
    {
        auto* waterRenderSystem{ dynamic_cast<ecs::system::WaterRenderSystem*>(&handle.get()) };

        waterRenderSystem->RenderReflectionTexture();
        waterRenderSystem->RenderRefractionTexture();
    }

    for (auto* renderer : m_rendererArray)
    {
        renderer->PrepareRendering();
        renderer->Render();
        renderer->FinishRendering();
    }
}

//-------------------------------------------------
// Lua config
//-------------------------------------------------

void sg::ogl::scene::Scene::ConfigSceneFromFile()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::ConfigSceneFromFile()] Loading scene from {}.", m_configFileName);

    InitLua();
    RunLuaScript();
}

void sg::ogl::scene::Scene::InitLua()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

    // vec3

    m_lua.new_usertype<glm::vec3>(
        "vec3",
        sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z,
        sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator/)
    );

    // Application

    m_lua["app"] = GetApplicationContext();

    // FirstPersonCamera

    m_lua.new_usertype<camera::FirstPersonCamera>("FirstPersonCamera",
        sol::constructors<camera::FirstPersonCamera(const std::string&, Application*), camera::FirstPersonCamera(const std::string&, Application*, const glm::vec3&, float, float)>(),
        "new", sol::factories([&](const std::string& t_name, Application* t_app, const glm::vec3& t_pos, float t_yaw, float t_pitch) -> std::shared_ptr<camera::FirstPersonCamera>
        {
            auto camera{ std::make_shared<camera::FirstPersonCamera>(t_name, t_app, t_pos, t_yaw, t_pitch) };
            m_cameras.emplace(t_name, camera);
            return camera;
        }),
        "SetMouseSensitivity", &camera::FirstPersonCamera::SetMouseSensitivity
    );

    // ThirdPersonCamera

    m_lua.new_usertype<camera::ThirdPersonCamera>("ThirdPersonCamera",
        sol::constructors<camera::ThirdPersonCamera(const std::string&, Application*, const glm::vec3&)>(),
        "new", sol::factories([&](const std::string& t_name, Application* t_app, const glm::vec3& t_pos) -> std::shared_ptr<camera::ThirdPersonCamera>
        {
            auto camera{ std::make_shared<camera::ThirdPersonCamera>(t_name, t_app, t_pos) };
            m_cameras.emplace(t_name, camera);
            return camera;
        }),
        "SetPlayerPosition", &camera::ThirdPersonCamera::SetPlayerPosition,
        "SetPlayerRotationY", &camera::ThirdPersonCamera::SetPlayerRotationY
    );

    // SetCurrentCamera functions

    m_lua.set_function("SetFirstPersonCameraAsCurrent", &Scene::SetFirstPersonCameraAsCurrent);
    m_lua.set_function("SetThirdPersonCameraAsCurrent", &Scene::SetThirdPersonCameraAsCurrent);

    // Scene

    m_lua["scene"] = std::ref(*this);
}

void sg::ogl::scene::Scene::RunLuaScript()
{
    auto fx{ m_lua.script_file(m_configFileName) };

    Log::SG_OGL_CORE_LOG_DEBUG("");
}
