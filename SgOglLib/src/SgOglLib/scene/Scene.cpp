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

void sg::ogl::scene::Scene::SetCurrentCameraByName(const std::string& t_name)
{
    SG_OGL_CORE_ASSERT(!cameras.empty(), "[Scene::SetCurrentCameraByName()] No cameras available.");

    if (cameras.count(t_name) != 0)
    {
        Log::SG_OGL_CORE_LOG_DEBUG("[SetCurrentCameraByName()] Set Camera {} as current.", t_name);
        SetCurrentCamera(cameras.at(t_name).get());
    }
    else
    {
        Log::SG_OGL_CORE_LOG_WARN("[SetCurrentCameraByName()] Camera {} not found.", t_name);
    }
}

void sg::ogl::scene::Scene::SetCurrentCamera(camera::Camera* t_camera)
{
    SG_OGL_CORE_ASSERT(t_camera, "[Scene::SetCurrentCamera()] Null pointer.");
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

    for (auto& r : renderer)
    {
        r->Update(t_dt);
    }
}

void sg::ogl::scene::Scene::Render()
{
    /*
    auto handle{ SceneCache::rendererCache.load<RenderSystemLoader<ecs::system::WaterRenderSystem>>("WaterRenderSystem"_hs,this) };
    if (handle)
    {
        auto* waterRenderSystem{ dynamic_cast<ecs::system::WaterRenderSystem*>(&handle.get()) };

        waterRenderSystem->RenderReflectionTexture();
        waterRenderSystem->RenderRefractionTexture();
    }
    */

    for (auto& r : renderer)
    {
        r->PrepareRendering();
        r->Render();
        r->FinishRendering();
    }
}
