// This file is part of the SgOgl package.
// 
// Filename: GameState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "GameState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    if (GetApplicationContext()->GetMouseInput().IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput().GetDisplVec());
    }

    return true;
}

bool GameState::Update(const double t_dt)
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

void GameState::Render()
{
    m_modelRenderSystem->Render();
    m_modelNormalmapRenderSystem->Render();
    m_skydomeRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(-10.0f, 10.0f, -20.0f), 74.0f, -9.0f);
    m_camera->SetMouseSensitivity(0.01f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // set ambient
    m_scene->SetAmbientIntensity(glm::vec3(0.3f));

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(-0.5f, -1.0f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.8f, 0.8f, 0.8f);
    m_sun->specularIntensity = glm::vec3(0.8f, 0.8f, 0.8f);
    m_scene->SetDirectionalLight(m_sun);

    // create and add a point light to the scene
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(0.0f, 10.0f, 0.0f);
    m_pointLight->position.y += 20.0f;
    m_pointLight->diffuseIntensity = glm::vec3(1.0f, 0.57f, 0.16f);
    m_scene->SetPointLight(m_pointLight);

    // create render systems
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
    m_modelNormalmapRenderSystem = std::make_unique<ModelNormalmapRenderSystem<ModelNormalmapShaderProgram>>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<SkydomeRenderSystem<DomeShaderProgram>>(m_scene.get());

    /*
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Bush_01/Bush_01.obj",
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        false,
        false,
        false
    );
    */

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/OldHouse/Big_Old_House.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        true,
        false,
        false
    );

    GetApplicationContext()->GetEntityFactory().CreateSkydomeEntity("res/model/dome/dome.obj");
}
