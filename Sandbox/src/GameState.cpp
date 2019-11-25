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
    m_waterRenderSystem->Update(t_dt);
    m_mousePicker->Update();

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

    // move the lamp model with mouse and change point light position
    auto view = m_scene->GetApplicationContext()->registry.view<
        sg::ogl::ecs::component::MoveableComponent,
        sg::ogl::ecs::component::ModelComponent,
        sg::ogl::ecs::component::TransformComponent>();

    for (auto entity : view)
    {
        if (GetApplicationContext()->GetMouseInput().IsLeftButtonPressed())
        {
            auto& transformComponent = view.get<sg::ogl::ecs::component::TransformComponent>(entity);
            transformComponent.position = m_mousePicker->GetCurrentTerrainPoint();

            m_pointLight->position = transformComponent.position;

            SG_OGL_CORE_LOG_DEBUG("x: {}  y: {}  z: {}", m_mousePicker->GetCurrentTerrainPoint().x, m_mousePicker->GetCurrentTerrainPoint().y, m_mousePicker->GetCurrentTerrainPoint().z);
        }
    }

    return true;
}

void GameState::Render()
{
    // render to textures
    m_waterRenderSystem->RenderReflectionTexture(m_modelRenderSystem, m_terrainRenderSystem, m_skyboxRenderSystem);
    m_waterRenderSystem->RenderRefractionTexture(m_modelRenderSystem, m_terrainRenderSystem, m_skyboxRenderSystem);

    //render to the screen
    m_modelRenderSystem->Render();
    m_terrainRenderSystem->Render();
    //m_instancingRenderSystem->Render();
    m_waterRenderSystem->Render();

    m_skyboxRenderSystem->Render();
    //m_skydomeRenderSystem->Render();

    //m_guiRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(-1163.0f, 119.0f, -2246.0f), 74.0f, -9.0f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // set ambient
    m_scene->SetAmbientIntensity(glm::vec3(0.2f));

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(-0.5f, -1.0f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.125f, 0.125f, 0.125f);
    m_sun->specularIntensity = glm::vec3(0.125f, 0.13f, 0.125f);
    m_scene->SetDirectionalLight(m_sun);

    // create and add a point light to the scene
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = m_lampPosition;
    m_pointLight->diffuseIntensity = glm::vec3(6.0f, 0.57f, 0.16f);
    m_scene->SetPointLight(m_pointLight);

    // create terrain
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(GetApplicationContext(), "res/config/Terrain.xml");
    m_terrain->GenerateTerrain<ComputeNormalmap, ComputeSplatmap>();

    // create water
    m_water = std::make_shared<sg::ogl::water::Water>(
        GetApplicationContext(),
        -400.0f, -2300.0f,
        WATER_HEIGHT,
        glm::vec3(750.0f * 2.0f, 750.0f, 750.0f),
        "res/texture/water/waterDUDV.png",
        "res/texture/water/normal.png"
    );

    // create render systems
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
    m_skyboxRenderSystem = std::make_unique<SkyboxRenderSystem<SkyboxShaderProgram>>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<SkydomeRenderSystem<DomeShaderProgram>>(m_scene.get());
    m_terrainRenderSystem = std::make_unique<TerrainRenderSystem<TerrainShaderProgram>>(m_scene.get());
    m_guiRenderSystem = std::make_unique<GuiRenderSystem<GuiShaderProgram>>(m_scene.get());
    m_instancingRenderSystem = std::make_unique<InstancingRenderSystem<InstancingShaderProgram>>(m_scene.get());
    m_waterRenderSystem = std::make_unique<sg::ogl::ecs::system::WaterRenderSystem<WaterShaderProgram>>(m_scene.get());

    // create house entity
    auto height{ m_terrain->GetHeightAtWorldPosition(-1000.0f, -2000.0f) };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/House/farmhouse_obj.obj",
        glm::vec3(-1000.0f, height, -2000.0f),
        glm::vec3(0.0f),
        glm::vec3(2.0f),
        false,
        false
    );

    // create tree entity
    height = m_terrain->GetHeightAtWorldPosition(-1090.0f, -2060.0f);
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Tree_02/tree02.obj",
        glm::vec3(-1090.0f, height, -2060.0f),
        glm::vec3(0.0f),
        glm::vec3(48.0f),
        false,
        false
    );

    // create lamp entity
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/lamp/Lamp.obj",
        m_lampPosition,
        glm::vec3(-90.0f, 0.0f, 0.0f),
        glm::vec3(0.125f),
        true,
        false
    );

    // create skybox entity
    /*
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };
    */
    const std::vector<std::string> cubemapFileNames{
        "res/texture/starfield/starfield_rt.jpg",
        "res/texture/starfield/starfield_lf.jpg",
        "res/texture/starfield/starfield_up.jpg",
        "res/texture/starfield/starfield_dn.jpg",
        "res/texture/starfield/starfield_bk.jpg",
        "res/texture/starfield/starfield_ft.jpg"
    };
    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);

    // create skydome entity
    GetApplicationContext()->GetEntityFactory().CreateSkydomeEntity("res/model/dome/dome.obj");

    // create terrain entity
    GetApplicationContext()->GetEntityFactory().CreateTerrainEntity(m_terrain);

    // crate water entity
    GetApplicationContext()->GetEntityFactory().CreateWaterEntity(m_water);

    // create gui entity
    const auto posX{ 0.5f };
    const auto posY{ 0.5f };
    const auto scaleX{ 0.25f };
    const auto scaleY{ 0.25f };
    GetApplicationContext()->GetEntityFactory().CreateGuiEntity(
        posX, posY,
        scaleX, scaleY,
        m_water->GetWaterFbos().GetReflectionColorTextureId()
    );

    // create plant entity (instancing)
    const uint32_t instances{ 10000 };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        instances,
        "res/model/Grass/grassmodel.obj",
        CreatePlantPositions(instances),
        true
    );

    // create mouse picker
    m_mousePicker = std::make_unique<sg::ogl::input::MousePicker>(m_scene.get(), m_terrain.get());
}

std::vector<glm::mat4> GameState::CreatePlantPositions(const uint32_t t_instances) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> posX(-1100.0f, -800.0f);
    const std::uniform_real_distribution<float> posZ(-2050.0f, -1900.0);

    std::vector<glm::mat4> matrices;

    for (auto i{ 0u }; i < t_instances; ++i)
    {
        sg::ogl::math::Transform transform;

        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        const auto height{ m_terrain->GetHeightAtWorldPosition(pos.x, pos.z) };

        transform.position = glm::vec3(pos.x, height, pos.z);
        transform.scale = glm::vec3(2.0f);

        matrices.push_back(transform.GetModelMatrix());
    }

    return matrices;
}
