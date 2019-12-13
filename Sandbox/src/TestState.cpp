// This file is part of the SgOgl package.
// 
// Filename: TestState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "TestState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool TestState::Input()
{
    if (GetApplicationContext()->GetMouseInput().IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput().GetDisplVec());
    }

    return true;
}

bool TestState::Update(const double t_dt)
{
    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_W))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::FORWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_S))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::BACKWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_A))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::LEFT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_D))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::RIGHT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_O))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::UP, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_U))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::DOWN, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_P))
    {
        SG_OGL_LOG_INFO("Camera x: {}  y: {}  z: {}", m_scene->GetCurrentCamera().GetPosition().x, m_scene->GetCurrentCamera().GetPosition().y, m_scene->GetCurrentCamera().GetPosition().z);
        SG_OGL_LOG_INFO("Camera yaw: {}  pitch: {}", m_scene->GetCurrentCamera().GetYaw(), m_scene->GetCurrentCamera().GetPitch());
    }

    return true;
}

void TestState::Render()
{
    m_skeletalModelRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TestState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(-2.7f, 99.0f, 225.0f), -86.0f, 1.6f);
    m_camera->SetMouseSensitivity(0.05f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // set ambient
    m_scene->SetAmbientIntensity(glm::vec3(0.2f));

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(0.9f, -0.1f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.3f, 0.2f, 0.1f);
    m_sun->specularIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
    m_scene->SetDirectionalLight(m_sun);

    // create and add a point light to the scene
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(91.0f, 165.0f, -42.0f);
    m_pointLight->diffuseIntensity = glm::vec3(1.0f, 0.57f, 10.16f);
    m_pointLight->linear = 0.007f;
    m_pointLight->quadratic = 0.0002f;
    m_scene->SetPointLight(m_pointLight);

    // create render systems
    m_skeletalModelRenderSystem = std::make_unique<sg::ogl::ecs::system::SkeletalModelRenderSystem>(m_scene.get());

    ////////////////// Create Entities //////////////////

    GetApplicationContext()->GetEntityFactory().CreateSkeletalModelEntity(
        "res/model/CastleGuard01/Idle.dae",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        false,
        false,
        true,
        false
    );
}
