// This file is part of the SgOgl package.
// 
// Filename: Scene.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "Scene.h"
#include "Core.h"
#include "Log.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "camera/Camera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[Scene::Scene()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[Scene::Scene()] Create Scene.");
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Scene::~Scene()] Destruct Scene.");
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
    return *m_currentCamera;
}

const sg::ogl::camera::Camera& sg::ogl::scene::Scene::GetCurrentCamera() const noexcept
{
    return *m_currentCamera;
}

sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetDirectionalLight() noexcept
{
    return *m_directionalLight;
}

const sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetDirectionalLight() const noexcept
{
    return *m_directionalLight;
}

const sg::ogl::scene::Scene::PointLightContainer& sg::ogl::scene::Scene::GetPointLights() const noexcept
{
    return m_pointLights;
}

bool sg::ogl::scene::Scene::IsDirectionalLight() const
{
    return m_directionalLight != nullptr;
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

void sg::ogl::scene::Scene::SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight)
{
    if (m_directionalLight)
    {
        m_directionalLight.reset();
    }

    m_directionalLight = t_directionalLight;
}

void sg::ogl::scene::Scene::AddPointLight(const PointLightSharedPtr& t_pointLight)
{
    SG_OGL_CORE_ASSERT(t_pointLight, "[Scene::AddPointLight()] Null pointer.")
    m_pointLights.push_back(t_pointLight);
}

void sg::ogl::scene::Scene::SetCurrentClipPlane(const glm::vec4& t_currentClipPlane)
{
    m_currentClipPlane = t_currentClipPlane;
}
