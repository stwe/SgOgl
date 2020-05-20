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
#include <assimp/postprocess.h>
#include "Scene.h"
#include "Core.h"
#include "Application.h"
#include "math/Transform.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "light/Sun.h"
#include "resource/ModelManager.h"
#include "camera/Camera.h"
#include "camera/FirstPersonCamera.h"
#include "camera/ThirdPersonCamera.h"
#include "lua/LuaHelper.h"
#include "ecs/component/Components.h"
#include "ecs/system/ForwardRenderSystem.h"
#include "ecs/system/DeferredRenderSystem.h"
#include "ecs/system/SkyboxRenderSystem.h"
#include "ecs/system/SunRenderSystem.h"

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

    for (auto& renderer : m_renderer)
    {
        renderer->Update(t_dt);
    }
}

void sg::ogl::scene::Scene::Render()
{
    for (auto& renderer : m_renderer)
    {
        renderer->PrepareRendering();
        renderer->Render();
        renderer->FinishRendering();
    }
}

//-------------------------------------------------
// Lua config file
//-------------------------------------------------

void sg::ogl::scene::Scene::AddCamera(lua_State* t_luaState, const std::string& t_cameraName)
{
    const auto cameras{ luabridge::getGlobal(t_luaState, "cameras") };
    const auto camera{ cameras[t_cameraName] };
    const auto cameraType{ camera["type"].cast<std::string>() };

    if (cameraType == "first")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddCamera()] Add First-Person-Camera {} to the scene.", t_cameraName);

        const auto position{ camera["position"] };
        auto firstCamera{ std::make_unique<camera::FirstPersonCamera>(
            GetApplicationContext(),
            glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
            camera["yaw"].cast<float>(),
            camera["pitch"].cast<float>()
        ) };

        firstCamera->SetCameraVelocity(camera["cameraVelocity"].cast<float>());
        firstCamera->SetMouseSensitivity(camera["mouseSensitivity"].cast<float>());
        firstCamera->name = t_cameraName;

        m_cameras.emplace(t_cameraName, std::move(firstCamera));

        const auto current{ camera["current"].cast<bool>() };
        if (current)
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddCamera()] Use {} camera as current.", t_cameraName);
            SetCurrentCamera(m_cameras.at(t_cameraName));
        }
    }

    if (cameraType == "third")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddCamera()] Add 3rd-Person-Camera {} to the scene.", t_cameraName);

        const auto playerPosition{ camera["playerPosition"] };
        auto thirdCamera{ std::make_unique<camera::ThirdPersonCamera>(
            GetApplicationContext(),
            glm::vec3(playerPosition["x"].cast<float>(), playerPosition["y"].cast<float>(), playerPosition["z"].cast<float>())
        ) };

        thirdCamera->SetCameraVelocity(camera["cameraVelocity"].cast<float>());
        thirdCamera->name = t_cameraName;

        m_cameras.emplace(t_cameraName, std::move(thirdCamera));

        const auto current{ camera["current"].cast<bool>() };
        if (current)
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddCamera()] Use {} camera as current.", t_cameraName);
            SetCurrentCamera(m_cameras.at(t_cameraName));
        }
    }
}

void sg::ogl::scene::Scene::AddEntity(lua_State* t_luaState, const std::string& t_entityName) const
{
    Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add entity {} to the scene.", t_entityName);

    auto componentKeys{ lua::LuaHelper::GetTableKeys(t_luaState, "entities." + t_entityName) };

    const auto entities{ luabridge::getGlobal(t_luaState, "entities") };
    const auto entity{ entities[t_entityName] };

    // create an entity
    const auto e{ m_application->registry.create() };

    // add components
    for (const auto& componentKey : componentKeys)
    {
        if (componentKey == "ModelComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add ModelComponent to the entity {}.", t_entityName);

            // get model component config
            const auto modelComponent{ entity["ModelComponent"] };

            // add model component
            const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_FlipUVs };
            m_application->registry.emplace<ecs::component::ModelComponent>(
                e,
                m_application->GetModelManager().GetModelByPath(modelComponent["path"].cast<std::string>(), pFlags),
                modelComponent["showTriangles"].cast<bool>(),
                modelComponent["fakeNormals"].cast<bool>()
            );
        }

        if (componentKey == "TransformComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add TransformComponent to the entity {}.", t_entityName);

            // get transform component config
            const auto transformComponent{ entity["TransformComponent"] };

            // add transform component
            auto position{ transformComponent["position"] };
            auto rotation{ transformComponent["rotation"] };
            auto scale{ transformComponent["scale"] };

            m_application->registry.emplace<math::Transform>(
                e,
                glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                glm::vec3(rotation["x"].cast<float>(), rotation["y"].cast<float>(), rotation["z"].cast<float>()),
                glm::vec3(scale["x"].cast<float>(), scale["y"].cast<float>(), scale["z"].cast<float>())
            );
        }

        if (componentKey == "PointLightComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add PointLightComponent to the entity {}.", t_entityName);

            // get point light component config
            const auto pointLightComponent{ entity["PointLightComponent"] };

            // add point light component
            auto position{ pointLightComponent["position"] };
            auto ambient{ pointLightComponent["ambientIntensity"] };
            auto diffuse{ pointLightComponent["diffuseIntensity"] };
            auto specular{ pointLightComponent["specularIntensity"] };

            m_application->registry.emplace<light::PointLight>(
                e,
                glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                glm::vec3(ambient["x"].cast<float>(), ambient["y"].cast<float>(), ambient["z"].cast<float>()),
                glm::vec3(diffuse["x"].cast<float>(), diffuse["y"].cast<float>(), diffuse["z"].cast<float>()),
                glm::vec3(specular["x"].cast<float>(), specular["y"].cast<float>(), specular["z"].cast<float>()),
                pointLightComponent["constant"].cast<float>(),
                pointLightComponent["linear"].cast<float>(),
                pointLightComponent["quadratic"].cast<float>()
            );
        }

        if (componentKey == "CubemapComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add CubemapComponent to the entity {}.", t_entityName);

            // get cubemap component config
            const auto cubemapComponent{ entity["CubemapComponent"] };

            std::vector<std::string> files;
            files.push_back(cubemapComponent["right"].cast<std::string>());
            files.push_back(cubemapComponent["left"].cast<std::string>());
            files.push_back(cubemapComponent["up"].cast<std::string>());
            files.push_back(cubemapComponent["down"].cast<std::string>());
            files.push_back(cubemapComponent["back"].cast<std::string>());
            files.push_back(cubemapComponent["front"].cast<std::string>());

            // add cubemap component
            m_application->registry.emplace<ecs::component::CubemapComponent>(
                e,
                m_application->GetTextureManager().GetCubemapId(files)
            );
        }

        if (componentKey == "DirectionalLightComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add DirectionalLightComponent to the entity {}.", t_entityName);

            // get directional light component config
            const auto directionalLightComponent{ entity["DirectionalLightComponent"] };

            // add directional light component
            auto position{ directionalLightComponent["direction"] };
            auto diffuse{ directionalLightComponent["diffuseIntensity"] };
            auto specular{ directionalLightComponent["specularIntensity"] };

            m_application->registry.emplace<light::DirectionalLight>(
                e,
                glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                glm::vec3(diffuse["x"].cast<float>(), diffuse["y"].cast<float>(), diffuse["z"].cast<float>()),
                glm::vec3(specular["x"].cast<float>(), specular["y"].cast<float>(), specular["z"].cast<float>())
            );
        }

        if (componentKey == "SunComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add SunComponent to the entity {}.", t_entityName);

            // get sun component config
            const auto sunComponent{ entity["SunComponent"] };

            // add sun component
            auto position{ sunComponent["direction"] };
            auto diffuse{ sunComponent["diffuseIntensity"] };
            auto specular{ sunComponent["specularIntensity"] };
            auto sunTextureId{ GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(sunComponent["sunTexturePath"].cast<std::string>()) };

            m_application->registry.emplace<light::Sun>(
                e,
                glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                glm::vec3(diffuse["x"].cast<float>(), diffuse["y"].cast<float>(), diffuse["z"].cast<float>()),
                glm::vec3(specular["x"].cast<float>(), specular["y"].cast<float>(), specular["z"].cast<float>()),
                sunTextureId,
                sunComponent["scale"].cast<float>()
            );
        }
    }
}

void sg::ogl::scene::Scene::AddRenderer(const int t_priority, const std::string& t_rendererName)
{
    if (t_rendererName == "ForwardRenderSystem")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddRenderer()] Add renderer {} to the scene.", t_rendererName);
        m_renderer.emplace_back(std::make_unique<ecs::system::ForwardRenderSystem>(t_priority, this));
    }

    if (t_rendererName == "DeferredRenderSystem")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddRenderer()] Add renderer {} to the scene.", t_rendererName);
        m_renderer.emplace_back(std::make_unique<ecs::system::DeferredRenderSystem>(t_priority, this));
    }

    if (t_rendererName == "SkyboxRenderSystem")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddRenderer()] Add renderer {} to the scene.", t_rendererName);
        m_renderer.emplace_back(std::make_unique<ecs::system::SkyboxRenderSystem>(t_priority, this));
    }

    if (t_rendererName == "SunRenderSystem")
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddRenderer()] Add renderer {} to the scene.", t_rendererName);
        m_renderer.emplace_back(std::make_unique<ecs::system::SunRenderSystem>(t_priority, this));
    }
}

void sg::ogl::scene::Scene::ConfigSceneFromFile()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Scene::ConfigSceneFromFile()] Loading scene from {}.", m_configFileName);

    // new lua state
    auto* luaState{ luaL_newstate() };
    luaopen_base(luaState);
    luaL_openlibs(luaState);

    // read scene config file
    lua::LuaHelper::LoadScript(luaState, m_configFileName);

    {
        // scene
        const auto sceneTable{ luabridge::getGlobal(luaState, "scene") };
        for (const auto& pair : pairs(sceneTable))
        {
            if (pair.first.cast<std::string>() == "ambientIntensity")
            {
                const auto scene{ luabridge::getGlobal(luaState, "scene") };
                const auto ambientIntensity{ scene["ambientIntensity"] };
                SetAmbientIntensity(glm::vec3(ambientIntensity["x"].cast<float>(), ambientIntensity["y"].cast<float>(), ambientIntensity["z"].cast<float>()));
            }
        }

        // add cameras
        auto cameraKeys{ lua::LuaHelper::GetTableKeys(luaState, "cameras") };
        for (const auto& cameraKey : cameraKeys)
        {
            AddCamera(luaState, cameraKey);
        }

        // add entities
        auto entityKeys{ lua::LuaHelper::GetTableKeys(luaState, "entities") };
        for (const auto& entityKey : entityKeys)
        {
            AddEntity(luaState, entityKey);
        }

        // add renderer
        const auto rendererTable{ luabridge::getGlobal(luaState, "renderer") };
        for (const auto& pair : pairs(rendererTable))
        {
            const auto priority{ rendererTable[pair.first.cast<std::string>()]["priority"].cast<int>() };
            const auto name{ rendererTable[pair.first.cast<std::string>()]["name"].cast<std::string>() };

            AddRenderer(priority, name);
        }
    }

    // sort renderer by priority
    std::sort(m_renderer.begin(), m_renderer.end(), [](const auto& t_lhs, const auto& t_rhs)
    {
        return t_lhs->priority > t_rhs->priority;
    });

    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] ---------------------------");
    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] Renderer priority settings.");
    auto i{ 0 };
    for (const auto& renderer : m_renderer)
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] {}. {}", i, renderer->debugName);
        i++;
    }
    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] ---------------------------");

    lua_close(luaState);
}
