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
    m_particleRenderSystem->Update(t_dt);

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


    ////////////////////////////////

    // get actual mouse ray
    m_mouseRay = m_mousePicker->GetRayFromMouse(
        static_cast<float>(GetApplicationContext()->GetMouseInput().GetCurrentPos().x),
        static_cast<float>(GetApplicationContext()->GetMouseInput().GetCurrentPos().y)
    );

    //SG_OGL_CORE_LOG_DEBUG("x: {}  y: {}  z: {}", m_mouseRay.x, m_mouseRay.y, m_mouseRay.z);

    // check for collision
    const auto result{ m_mousePicker->RaySphere(
        m_scene->GetCurrentCamera().GetPosition(),
        m_mouseRay,
        m_spherePosition,
        m_sphereRadius,
        &m_dist
    ) };

    auto view = m_scene->GetApplicationContext()->registry.view<
        sg::ogl::ecs::component::SphereComponent,
        sg::ogl::ecs::component::ModelComponent,
        sg::ogl::ecs::component::TransformComponent>();

    for (auto entity : view)
    {
        // get pos from entity and change if collision
        if (result && GetApplicationContext()->GetMouseInput().IsLeftButtonPressed())
        {
            auto& transformComponent = view.get<sg::ogl::ecs::component::TransformComponent>(entity);

            transformComponent.position.x += GetApplicationContext()->GetMouseInput().GetDisplVec().x * t_dt * 8.0f;
            transformComponent.position.y += -GetApplicationContext()->GetMouseInput().GetDisplVec().y * t_dt * 8.0f;
            m_spherePosition = transformComponent.position;

            SG_OGL_CORE_LOG_DEBUG("Result: {}, Dist: {}", result, m_dist);
        }
    }

    ////////////////////////////////


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
    m_instancingRenderSystem->Render();
    m_waterRenderSystem->Render();
    m_particleRenderSystem->Render();

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

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(1000.0f, 1000.0f, 1000.0f);
    m_sun->diffuseIntensity = glm::vec3(1.3f, 1.3f, 1.3f);
    m_scene->SetDirectionalLight(m_sun);

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
    m_particleRenderSystem = std::make_unique<ParticleRenderSystem<ParticleShaderProgram>>(m_scene.get());

    // create house entity
    auto height{ m_terrain->GetHeightAtWorldPosition(-1000.0f, -2000.0f) };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/House/farmhouse_obj.obj",
        glm::vec3(-1000.0f, height, -2000.0f),
        glm::vec3(0.0f),
        glm::vec3(2.0f)
    );

    // create sphere entity
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/sphere/sphere.obj",
        m_spherePosition,
        glm::vec3(0.0f),
        glm::vec3(m_sphereRadius),
        true
    );

    // create tree entity
    height = m_terrain->GetHeightAtWorldPosition(-1090.0f, -2060.0f);
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Tree_02/tree02.obj",
        glm::vec3(-1090.0f, height, -2060.0f),
        glm::vec3(0.0f),
        glm::vec3(64.0f)
    );

    // create skybox entity
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
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
        CreatePlantPositions(instances)
    );

    // change particle random generator setup
    sg::ogl::particle::BuildConfig buildConfig;
    buildConfig.yRange.x = 6.0f;
    buildConfig.yRange.y = 10.0f;

    // create smoke entity (instancing - particle system)
    m_particleEmitter1 = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        glm::vec3(-1000.0f, 100.0f, -2000.0f), // root position
        400,                                            // max particles
        2,                                              // new particles
        "res/texture/particle/smoke.png",
        8                                               // number of rows
    );
    m_particleEmitter1->SetGravityEffect(0.0f);
    m_particleEmitter1->SetBuildConfig(buildConfig);
    GetApplicationContext()->GetEntityFactory().CreateParticleEntity(m_particleEmitter1);

    // create fire entity (instancing - particle system)
    m_particleEmitter2 = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        glm::vec3(-1163.0f, 65.0f, -2036.0f), // root position
        400,                                           // max particles
        2,                                             // new particles
        "res/texture/particle/fire.png",
        8                                              // number of rows
    );
    m_particleEmitter2->SetGravityEffect(0.0f);
    m_particleEmitter2->SetBuildConfig(buildConfig);
    GetApplicationContext()->GetEntityFactory().CreateParticleEntity(m_particleEmitter2);

    // create mouse picker
    m_mousePicker = std::make_unique<sg::ogl::input::MousePicker>(m_scene.get());
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
