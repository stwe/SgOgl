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
    // todo

    const auto it{ m_rendererMap.find("WaterRenderSystem") };
    if (it != m_rendererMap.end())
    {
        auto* r = dynamic_cast<ecs::system::WaterRenderSystem*>(it->second.get());
        r->RenderReflectionTexture(m_rendererMap.at("SkydomeRenderSystem"), m_rendererMap.at("ForwardRenderSystem"));
        r->RenderRefractionTexture(m_rendererMap.at("SkydomeRenderSystem"), m_rendererMap.at("ForwardRenderSystem"));
    }

    for (auto& renderer : m_rendererArray)
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

void sg::ogl::scene::Scene::AddEntity(lua_State* t_luaState, const std::string& t_entityName)
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

            const auto modelComponent{ entity["ModelComponent"] };

            const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_FlipUVs };
            m_application->registry.emplace<ecs::component::ModelComponent>(
                e,
                m_application->GetModelManager().GetModelByPath(modelComponent["path"].cast<std::string>(), pFlags),
                modelComponent["showTriangles"].cast<bool>()
            );
        }

        if (componentKey == "TransformComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add TransformComponent to the entity {}.", t_entityName);

            const auto transformComponent{ entity["TransformComponent"] };

            auto position{ transformComponent["position"] };

            auto rotation{ glm::vec3(0.0f) };
            if (!transformComponent["rotation"].isNil())
            {
                auto r{ transformComponent["rotation"] };
                rotation.x = r["x"].cast<float>();
                rotation.y = r["y"].cast<float>();
                rotation.z = r["z"].cast<float>();
            }

            auto scale{ glm::vec3(1.0f) };
            if (!transformComponent["scale"].isNil())
            {
                auto s{ transformComponent["scale"] };
                scale.x = s["x"].cast<float>();
                scale.y = s["y"].cast<float>();
                scale.z = s["z"].cast<float>();
            }

            m_application->registry.emplace<math::Transform>(
                e,
                glm::vec3(position["x"].cast<float>(), position["y"].cast<float>(), position["z"].cast<float>()),
                rotation,
                scale
            );
        }

        if (componentKey == "PointLightComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add PointLightComponent to the entity {}.", t_entityName);

            const auto pointLightComponent{ entity["PointLightComponent"] };

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

            const auto cubemapComponent{ entity["CubemapComponent"] };

            std::vector<std::string> files;
            files.push_back(cubemapComponent["right"].cast<std::string>());
            files.push_back(cubemapComponent["left"].cast<std::string>());
            files.push_back(cubemapComponent["up"].cast<std::string>());
            files.push_back(cubemapComponent["down"].cast<std::string>());
            files.push_back(cubemapComponent["back"].cast<std::string>());
            files.push_back(cubemapComponent["front"].cast<std::string>());

            m_application->registry.emplace<ecs::component::CubemapComponent>(
                e,
                m_application->GetTextureManager().GetCubemapId(files)
            );
        }

        if (componentKey == "SkydomeComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add SkydomeComponent to the entity {}.", t_entityName);
            m_application->registry.emplace<ecs::component::SkydomeComponent>(e);
        }

        if (componentKey == "DirectionalLightComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add DirectionalLightComponent to the entity {}.", t_entityName);

            const auto directionalLightComponent{ entity["DirectionalLightComponent"] };

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

            const auto sunComponent{ entity["SunComponent"] };

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

        if (componentKey == "GuiComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add GuiComponent to the entity {}.", t_entityName);

            const auto guiComponent{ entity["GuiComponent"] };

            m_application->registry.emplace<ecs::component::GuiComponent>(
                e,
                m_application->GetTextureManager().GetTextureIdFromPath(guiComponent["guiTexturePath"].cast<std::string>())
            );
        }

        if (componentKey == "WaterComponent")
        {
            Log::SG_OGL_CORE_LOG_INFO("[Scene::AddEntity()] Add WaterComponent to the entity {}.", t_entityName);

            const auto waterComponent{ entity["WaterComponent"] };
            const auto tilesSize{ waterComponent["tileSize"] };

            // create a Water asset
            auto water{ std::make_unique<water::Water>(
                GetApplicationContext(),
                waterComponent["xPosition"].cast<float>(), waterComponent["zPosition"].cast<float>(),
                waterComponent["height"].cast<float>(),
                glm::vec3(tilesSize["x"].cast<float>(), tilesSize["y"].cast<float>(), tilesSize["z"].cast<float>()),
                waterComponent["dudvTexturePath"].cast<std::string>(),
                waterComponent["normalMapTexturePath"].cast<std::string>()
            ) };

            // store the Water asset
            m_waterContainer.emplace(t_entityName, std::move(water));

            // finally create an Entity
            m_application->registry.emplace<ecs::component::WaterComponent>(
                e,
                m_waterContainer.at(t_entityName)
            );
        }
    }
}

void sg::ogl::scene::Scene::AddRenderer(const int t_priority, const std::string& t_rendererName)
{
    auto add{ false };

    if (t_rendererName == "ForwardRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::ForwardRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "DeferredRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::DeferredRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "SkyboxRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::SkyboxRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "SkydomeRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::SkydomeRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "SunRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::SunRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "GuiRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::GuiRenderSystem>(t_priority, this));
        add = true;
    }

    if (t_rendererName == "WaterRenderSystem")
    {
        m_rendererMap.emplace(t_rendererName, std::make_unique<ecs::system::WaterRenderSystem>(t_priority, this));
        add = true;
    }

    if (add)
    {
        m_rendererArray.push_back(m_rendererMap.at(t_rendererName));
        Log::SG_OGL_CORE_LOG_INFO("[Scene::AddRenderer()] Added renderer {} to the scene.", t_rendererName);
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
    std::sort(m_rendererArray.begin(), m_rendererArray.end(), [](const auto& t_lhs, const auto& t_rhs)
    {
        return t_lhs->priority > t_rhs->priority;
    });

    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] ---------------------------");
    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] Renderer priority settings.");
    auto i{ 0 };
    for (const auto& renderer : m_rendererArray)
    {
        Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] {}. {}", i, renderer->debugName);
        i++;
    }
    Log::SG_OGL_CORE_LOG_INFO("[Scene::ConfigSceneFromFile()] ---------------------------");

    lua_close(luaState);
}
