// This file is part of the SgOgl package.
// 
// Filename: Scene.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <lua.hpp>
#include <LuaBridge.h>
#include "Scene.h"
#include "Core.h"
#include "Application.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "camera/Camera.h"
#include "SgOglException.h"
#include "camera/FirstPersonCamera.h"
#include "camera/ThirdPersonCamera.h"
#include "lua/LuaHelper.h"

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

const sg::ogl::scene::Scene::ScenePointLightContainer& sg::ogl::scene::Scene::GetScenePointLights() const noexcept
{
    return m_scenePointLights;
}

const sg::ogl::scene::Scene::EntityPointLightContainer& sg::ogl::scene::Scene::GetEntityPointLights() const noexcept
{
    return m_entityPointLights;
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

void sg::ogl::scene::Scene::AddScenePointLight(const PointLightSharedPtr& t_pointLight)
{
    SG_OGL_CORE_ASSERT(t_pointLight, "[Scene::AddScenePointLight()] Null pointer.");
    m_scenePointLights.push_back(t_pointLight);
}

void sg::ogl::scene::Scene::AddEntityPointLight(const std::string& t_name, const PointLightSharedPtr& t_pointLight)
{
    SG_OGL_CORE_ASSERT(!t_name.empty(), "[Scene::AddEntityPointLight()] Invalid name.");
    SG_OGL_CORE_ASSERT(t_pointLight, "[Scene::AddEntityPointLight()] Null pointer.");

    if (m_entityPointLights.count(t_name) == 0)
    {
        m_entityPointLights.emplace(t_name, t_pointLight);
        Log::SG_OGL_CORE_LOG_DEBUG("[Scene::AddEntityPointLight()] Entity Point Light {} added to the scene.", t_name);
    }
}

void sg::ogl::scene::Scene::SetCurrentClipPlane(const glm::vec4& t_currentClipPlane)
{
    m_currentClipPlane = t_currentClipPlane;
}

void sg::ogl::scene::Scene::ConfigSceneFromFile()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::ConfigSceneFromFile()] Loading scene from {}.", m_configFileName);

    auto* luaState{ luaL_newstate() };
    luaopen_base(luaState);
    luaL_openlibs(luaState);

    lua::LuaHelper::LoadScript(luaState, m_configFileName);

    auto cameraKeys{ lua::LuaHelper::GetTableKeys(luaState, "cameras") };

    // destroy LuaRef before lua_close
    {
        const auto entityTable = luabridge::getGlobal(luaState, "cameras");

        for (const auto& cameraKey : cameraKeys)
        {
            luabridge::LuaRef camera{ entityTable[cameraKey] };
            auto type{ camera["type"] };

            std::string typeStr;
            if (type.isString())
            {
                typeStr = type.cast<std::string>();
            }

            if (typeStr == std::string("first"))
            {
                auto velocity{ camera["cameraVelocity"] };
                auto sensitivity{ camera["mouseSensitivity"] };
                auto yaw{ camera["yaw"] };
                auto pitch{ camera["pitch"] };
                auto position{ camera["position"] };

                auto firstCamera{ std::make_unique<camera::FirstPersonCamera>(
                    GetApplicationContext(),
                    glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                    yaw.cast<float>(),
                    pitch.cast<float>()
                ) };

                firstCamera->SetCameraVelocity(velocity.cast<float>());
                firstCamera->SetMouseSensitivity(sensitivity.cast<float>());

                m_cameras.push_back(std::move(firstCamera));
            }
            else if (typeStr == std::string("third"))
            {
                auto velocity{ camera["cameraVelocity"] };
                auto playerPosition{ camera["playerPosition"] };

                auto thirdCamera{ std::make_unique<camera::ThirdPersonCamera>(
                    GetApplicationContext(),
                    glm::vec3(playerPosition["x"].cast<float>(), playerPosition["y"].cast<float>(), playerPosition["z"].cast<float>())
                ) };

                thirdCamera->SetCameraVelocity(velocity.cast<float>());

                m_cameras.push_back(std::move(thirdCamera));
            }
        }
    }

    lua_close(luaState);
}
