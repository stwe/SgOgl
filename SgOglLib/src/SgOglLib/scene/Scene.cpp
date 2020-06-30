// This file is part of the SgOgl package.
// 
// Filename: Scene.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Scene.h"
#include "Core.h"
#include "Application.h"
#include "input/MouseInput.h"
#include "camera/Camera.h"
#include "ecs/component/Components.h"
#include "ecs/system/WaterRenderSystem.h"
#include "particle/ParticleSystem.h"

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

void sg::ogl::scene::Scene::SetParentLuaState(lua_State* t_luaState)
{
    SG_OGL_CORE_ASSERT(t_luaState, "[Scene::SetParentLuaState()] Null pointer.");
    m_parentLuaState = t_luaState;
}

void sg::ogl::scene::Scene::SetCurrentClipPlane(const glm::vec4& t_currentClipPlane)
{
    m_currentClipPlane = t_currentClipPlane;
}

void sg::ogl::scene::Scene::SetAmbientIntensity(const glm::vec3& t_ambientIntensity)
{
    m_ambientIntensity = t_ambientIntensity;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Scene::Input()
{
    GetCurrentCamera().Input();

    // todo: tmp code

    // run the input components
    if (m_parentLuaState)
    {
        // todo in lua abfragen
        if (GetApplicationContext()->GetMouseInput().IsLeftButtonPressed())
        {
            auto mousePosition = GetApplicationContext()->GetMouseInput().GetCurrentPos();

            // ndc
            const auto x{ (2.0f * mousePosition.x) / static_cast<float>(GetApplicationContext()->GetProjectionOptions().width) - 1.0f };
            const auto y{ 1.0f - (2.0f * mousePosition.y) / static_cast<float>(GetApplicationContext()->GetProjectionOptions().height) };

            sol::state_view lua{ m_parentLuaState };
            m_application->registry.view<ecs::component::InputComponent>().each(
                [&](entt::entity t_entity, auto& t_inputComponent)
                {
                    lua[t_inputComponent.luaFunction](t_entity, x, y);
                    //lua[t_inputComponent.luaFunction](t_entity, 0.5f * x + 0.5f, 0.5f * y + 0.5f);
                }
            );
        }
    }
}

void sg::ogl::scene::Scene::Update(const double t_dt)
{
    GetCurrentCamera().Update(t_dt);

    // run the update function of all renderers
    for (auto& r : renderer)
    {
        r->Update(t_dt);
    }

    // run the update components
    if (m_parentLuaState)
    {
        sol::state_view lua{ m_parentLuaState };
        m_application->registry.view<ecs::component::UpdateComponent>().each(
            [&](entt::entity t_entity, auto& t_updateComponent)
            {
                lua[t_updateComponent.luaFunction](t_entity, t_dt);
            }
        );
    }
}

void sg::ogl::scene::Scene::Render()
{
    // todo: do not search for the debug name
    if (!waterSurfaces.empty())
    {
        const auto& it = std::find_if(renderer.begin(), renderer.end(),
            [](std::unique_ptr<ecs::system::RenderSystemInterface>& t_renderSystem) {return t_renderSystem->debugName == "WaterRenderer"; });

        if (it != renderer.end())
        {
            auto* waterRenderSystem{ dynamic_cast<ecs::system::WaterRenderSystem*>(it->get()) };
            waterRenderSystem->RenderReflectionTexture();
            waterRenderSystem->RenderRefractionTexture();
        }
    }

    for (auto& r : renderer)
    {
        r->PrepareRendering();
        r->Render();
        r->FinishRendering();
    }
}
