// This file is part of the SgOgl package.
// 
// Filename: GameState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "GameState.h"
#include "shader/ComputeSplatmap.h"
#include "shader/ComputeNormalmap.h"

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
    m_terrainRenderSystem->Render();
    //m_skydomeRenderSystem->Render();
    m_guiRenderSystem->Render();
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
    m_camera->SetPosition(glm::vec3(0.0f, 50.0f, -100.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create terrain object
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(GetApplicationContext(), "res/config/Terrain.xml");

    // generate terrain
    m_terrain->GenerateTerrain<ComputeNormalmap, ComputeSplatmap>();

    // create entities
    CreateHouseEntity();
    CreateSkyboxEntity();
    //CreateSkydomeEntity();
    CreateTerrainEntity();
    CreateGuiEntity();
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
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        m_houseEntity,
        glm::vec3(-10.0f, 14.0f, 0.0f)
    );

    // create a render system for the house entity
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
}

void GameState::CreateSkyboxEntity()
{
    // create an entity
    m_skyboxEntity = GetApplicationContext()->registry.create();

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        m_skyboxEntity,
        GetApplicationContext()->GetModelManager().GetStaticMeshByName(sg::ogl::resource::ModelManager::SKYBOX_MESH)
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

    // create a render system for the skybox entity
    m_skyboxRenderSystem = std::make_unique<SkyboxRenderSystem<SkyboxShaderProgram>>(m_scene.get());
}

void GameState::CreateSkydomeEntity()
{
    // create an entity
    m_skydomeEntity = GetApplicationContext()->registry.create();

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        m_skydomeEntity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/dome/dome.obj")
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        m_skydomeEntity,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(GetApplicationContext()->GetProjectionOptions().farPlane * 0.5f)
    );

    // add skydome component/tag
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::SkydomeComponent>(m_skydomeEntity);

    // create a render system for the skydome entity
    m_skydomeRenderSystem = std::make_unique<SkydomeRenderSystem<DomeShaderProgram>>(m_scene.get());
}

void GameState::CreateTerrainEntity()
{
    // create an entity
    m_terrainEntity = GetApplicationContext()->registry.create();

    // add terrain component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TerrainComponent>(
        m_terrainEntity,
        m_terrain
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        m_terrainEntity,
        glm::vec3(m_terrain->GetTerrainOptions().xPos, 0.0f, m_terrain->GetTerrainOptions().zPos)
    );

    // create a render system for the terrain entity
    m_terrainRenderSystem = std::make_unique<TerrainRenderSystem<TerrainShaderProgram>>(m_scene.get());
}

void GameState::CreateGuiEntity()
{
    // create an entity
    m_guiEntity = GetApplicationContext()->registry.create();

    const auto posX{ 0.5f };
    const auto posY{ 0.5f };

    const auto scaleX{ 0.25f };
    const auto scaleY{ 0.25f };

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        m_guiEntity,
        glm::vec3(posX, posY, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(scaleX, scaleY, 1.0f)
    );

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        m_guiEntity,
        GetApplicationContext()->GetModelManager().GetStaticMeshByName(sg::ogl::resource::ModelManager::GUI_MESH)
    );

    // add gui component
    /*
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::GuiComponent>(
        m_guiEntity,
        GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/texture/test.png")
    );
    */

    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::GuiComponent>(
        m_guiEntity,
        m_terrain->GetNormalmapTextureId()
    );

    // create a render system for the gui entity
    m_guiRenderSystem = std::make_unique<GuiRenderSystem<GuiShaderProgram>>(m_scene.get());
}
