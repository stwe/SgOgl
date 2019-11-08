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
    // enable clipping
    sg::ogl::OpenGl::EnableClipping();

    // render reflection texture
    RenderReflectionTexture();

    // render refraction texture
    RenderRefractionTexture();

    // disable clipping  -  render to the screen
    sg::ogl::OpenGl::DisableClipping();
    m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));

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
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(-1364.0f, 389.0f, -3112.0f), 74.0f, -25.0f);

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

    // create render systems
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
    m_skyboxRenderSystem = std::make_unique<SkyboxRenderSystem<SkyboxShaderProgram>>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<SkydomeRenderSystem<DomeShaderProgram>>(m_scene.get());
    m_terrainRenderSystem = std::make_unique<TerrainRenderSystem<TerrainShaderProgram>>(m_scene.get());
    m_guiRenderSystem = std::make_unique<GuiRenderSystem<GuiShaderProgram>>(m_scene.get());
    m_instancingRenderSystem = std::make_unique<InstancingRenderSystem<InstancingShaderProgram>>(m_scene.get());
    m_waterRenderSystem = std::make_unique<WaterRenderSystem<WaterShaderProgram>>(m_scene.get());

    // create a fbos object for water rendering
    m_waterFbos = std::make_shared<sg::ogl::buffer::WaterFbos>(GetApplicationContext());

    // create entities
    CreateHouseEntity();
    CreateTreeEntity();
    CreateSkyboxEntity();
    CreateSkydomeEntity();
    CreateTerrainEntity();
    CreateWaterEntity();
    CreateGuiEntity();

    //AddGrass(50000, "res/model/Grass/grassmodel.obj");
}

void GameState::CreateHouseEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/House/farmhouse_obj.obj")
    );

    const auto posX{ -1000.0f };
    const auto posZ{ -2000.0f };
    const auto height{ m_terrain->GetHeightAtWorldPosition(posX, posZ) };

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(posX, height, posZ),
        glm::vec3(0.0f),
        glm::vec3(2.0f)
    );
}

void GameState::CreateTreeEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/Tree_02/tree02.obj")
    );

    const auto posX{ -1090.0f };
    const auto posZ{ -2060.0f };
    const auto height{ m_terrain->GetHeightAtWorldPosition(posX, posZ) };

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(posX, height, posZ),
        glm::vec3(0.0f),
        glm::vec3(64.0f)
    );
}

void GameState::CreateSkyboxEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        entity,
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
        entity,
        GetApplicationContext()->GetTextureManager().GetCubemapId(cubemapFileNames)
    );
}

void GameState::CreateSkydomeEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/dome/dome.obj")
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(GetApplicationContext()->GetProjectionOptions().farPlane * 0.5f)
    );

    // add skydome component/tag
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::SkydomeComponent>(entity);
}

void GameState::CreateTerrainEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add terrain component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TerrainComponent>(
        entity,
        m_terrain
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(m_terrain->GetTerrainOptions().xPos, 0.0f, m_terrain->GetTerrainOptions().zPos)
    );
}

void GameState::CreateWaterEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetStaticMeshByName(sg::ogl::resource::ModelManager::WATER_MESH)
    );

    const auto xPos{ -400.0f };
    const auto zPos{ -2300.0f };
    const auto height{ WATER_HEIGHT };
    const auto tileSize{ 750.0f };

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(xPos, height, zPos),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(tileSize * 2.0f, tileSize, tileSize)
    );

    const auto dudvTextureId{ GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/texture/water/waterDUDV.png") };
    const auto normalTextureId{ GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/texture/water/normal.png") };

    // add water component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::WaterComponent>(
        entity,
        m_waterFbos->GetReflectionColorTextureId(),
        m_waterFbos->GetRefractionColorTextureId(),
        dudvTextureId,
        normalTextureId,
        m_waterFbos->GetRefractionDepthTextureId()
    );
}

void GameState::CreateGuiEntity()
{
    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    const auto posX{ 0.5f };
    const auto posY{ 0.5f };

    const auto scaleX{ 0.25f };
    const auto scaleY{ 0.25f };

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(
        entity,
        glm::vec3(posX, posY, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(scaleX, scaleY, 1.0f)
    );

    // add mesh component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::MeshComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetStaticMeshByName(sg::ogl::resource::ModelManager::GUI_MESH)
    );

    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::GuiComponent>(
        entity,
        m_waterFbos->GetReflectionColorTextureId()
    );
}

void GameState::AddGrass(const uint32_t t_instances, const std::string& t_path)
{
    std::vector<glm::mat4> matrices;

    CreateGrassPositions(t_instances, matrices);

    const unsigned int pFlags{ aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals };

    for (auto& mesh : GetApplicationContext()->GetModelManager().GetModelByPath(t_path, pFlags)->GetMeshes())
    {
        // get Vao of the mesh
        auto& vao{ mesh->GetVao() };

        // create an empty Vbo for instanced data
        const uint32_t numberOfFloatsPerInstance{ 16 };
        const auto vbo{ vao.AddEmptyVbo(numberOfFloatsPerInstance * t_instances) };

        // set Vbo attributes
        vao.AddInstancedAttribute(vbo, 5, 4, numberOfFloatsPerInstance, 0);
        vao.AddInstancedAttribute(vbo, 6, 4, numberOfFloatsPerInstance, 4);
        vao.AddInstancedAttribute(vbo, 7, 4, numberOfFloatsPerInstance, 8);
        vao.AddInstancedAttribute(vbo, 8, 4, numberOfFloatsPerInstance, 12);

        // store data
        vao.StoreTransformationMatrices(vbo, numberOfFloatsPerInstance * t_instances, matrices);
    }

    // create an entity
    const auto entity{ GetApplicationContext()->registry.create() };

    // add instances component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::InstancesComponent>(
        entity,
        t_instances
    );

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        entity,
        GetApplicationContext()->GetModelManager().GetModelByPath(t_path)
    );
}

void GameState::CreateGrassPositions(const uint32_t t_instances, std::vector<glm::mat4>& t_matrices) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> posX(1.0f, 550.0f);
    const std::uniform_real_distribution<float> posZ(-350.0f, 350.0f);

    for (auto i{ 0u }; i < t_instances; ++i)
    {
        sg::ogl::math::Transform transform;

        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        const auto height{ m_terrain->GetHeightAtWorldPosition(pos.x, pos.z) };

        transform.position = glm::vec3(pos.x, height, pos.z);
        transform.scale = glm::vec3(2.0f);

        t_matrices.push_back(transform.GetModelMatrix());
    }
}

void GameState::RenderReflectionTexture() const
{
    // render all above the water surface
    m_waterFbos->BindReflectionFboAsRenderTarget();

    const auto distance{ 2.0f * (m_camera->GetPosition().y - WATER_HEIGHT) };
    m_camera->GetPosition().y -= distance;
    m_camera->InvertPitch();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene->SetCurrentClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -WATER_HEIGHT));

    m_modelRenderSystem->Render();
    m_terrainRenderSystem->Render();
    //m_instancingRenderSystem->Render();

    m_skyboxRenderSystem->Render();
    //m_skydomeRenderSystem->Render();

    m_camera->GetPosition().y += distance;
    m_camera->InvertPitch();

    m_waterFbos->UnbindRenderTarget();
}

void GameState::RenderRefractionTexture() const
{
    // render all under the water surface
    m_waterFbos->BindRefractionFboAsRenderTarget();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, WATER_HEIGHT));

    m_modelRenderSystem->Render();
    m_terrainRenderSystem->Render();
    //m_instancingRenderSystem->Render();

    m_skyboxRenderSystem->Render();
    //m_skydomeRenderSystem->Render();

    m_waterFbos->UnbindRenderTarget();
}
