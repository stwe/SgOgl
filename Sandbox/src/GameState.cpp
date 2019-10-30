// This file is part of the SgOgl package.
// 
// Filename: GameState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

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

    return true;
}

void GameState::Render()
{
    m_modelRenderSystem->Render();
    m_skyboxRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(0.0f, 10.0f, -40.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create entities
    CreateHouseEntity();
    CreateSkyboxEntity();
}

void GameState::CreateHouseEntity()
{
    // create an entity
    m_houseEntity = GetApplicationContext()->registry.create();

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        m_houseEntity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/House/farmhouse_obj.obj")
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(m_houseEntity);

    // create a render system for the entity
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
}

void GameState::CreateSkyboxEntity()
{
    // create an entity
    m_skyboxEntity = GetApplicationContext()->registry.create();

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        m_skyboxEntity,
        GetApplicationContext()->GetModelManager().GetStaticMeshByName("skybox")
    );

    // add cubemap component
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };

    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::CubemapComponent>(
        m_skyboxEntity,
        GetApplicationContext()->GetTextureManager().GetCubemapId(cubemapFileNames)
    );

    // create a render system for the entity
    m_skyboxRenderSystem = std::make_unique<SkyboxRenderSystem<SkyboxShaderProgram>>(m_scene.get());
}
