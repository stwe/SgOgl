#include "GameState.h"
#include "DomeShaderProgram.h"
#include "SkyboxShaderProgram.h"
#include "ModelShaderProgram.h"
#include "ModelLightingShaderProgram.h"
#include "InstancingShaderProgram.h"
#include "InstancingLightingShader.h"
#include "TerrainShaderProgram.h"
#include "TerrainLightingShaderProgram.h"
#include "ParticleShaderProgram.h"
#include "ComputeNormalmap.h"
#include "ComputeSplatmap.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    if (GetApplicationContext()->GetMouseInput()->IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput()->GetDisplVec());
    }

    return true;
}

bool GameState::Update(const double t_dt)
{
#ifdef SKYDOME
    m_skydomeEntity->Update();
#else
    m_skyboxEntity->Update();
#endif

    m_terrainEntity->Update();
    m_houseEntity->Update();
    m_heroEntity->Update();
    m_particleGenerator->Update(t_dt);

    // Camera

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_W))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::FORWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_S))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::BACKWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_A))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::LEFT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_D))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::RIGHT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_O))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::UP, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_U))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::DOWN, t_dt * CAMERA_VELOCITY);
    }

    // Hero

    /*
    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_UP))
    {
        m_heroEntity->GetLocalTransform().position = glm::vec3(
            m_heroEntity->GetLocalTransform().position.x,
            m_terrain->GetHeightAtWorldPosition(m_heroEntity->GetLocalTransform().position.x, m_heroEntity->GetLocalTransform().position.z),
            m_heroEntity->GetLocalTransform().position.z + 0.5f
        );
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_DOWN))
    {
        m_heroEntity->GetLocalTransform().position = glm::vec3(
            m_heroEntity->GetLocalTransform().position.x,
            m_terrain->GetHeightAtWorldPosition(m_heroEntity->GetLocalTransform().position.x, m_heroEntity->GetLocalTransform().position.z),
            m_heroEntity->GetLocalTransform().position.z - 0.5f
        );
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_LEFT))
    {
        m_heroEntity->GetLocalTransform().position = glm::vec3(
            m_heroEntity->GetLocalTransform().position.x + 0.5f,
            m_terrain->GetHeightAtWorldPosition(m_heroEntity->GetLocalTransform().position.x, m_heroEntity->GetLocalTransform().position.z),
            m_heroEntity->GetLocalTransform().position.z
        );
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_RIGHT))
    {
        m_heroEntity->GetLocalTransform().position = glm::vec3(
            m_heroEntity->GetLocalTransform().position.x - 0.5f,
            m_terrain->GetHeightAtWorldPosition(m_heroEntity->GetLocalTransform().position.x, m_heroEntity->GetLocalTransform().position.z),
            m_heroEntity->GetLocalTransform().position.z
        );
    }
    */

    return true;
}

void GameState::Render()
{
    m_terrainEntity->Render();
    m_houseEntity->Render();
    m_heroEntity->Render();
    m_grassEntity->Render();
    m_particleGenerator->Render();

#ifdef SKYDOME
    m_skydomeEntity->Render();
#else
    m_skyboxEntity->Render();
#endif
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(370.0f, 100.0f, 370.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create a directional light
#ifdef DIRECTIONAL_LIGHTING
    m_directionalLight = std::make_shared<sg::ogl::light::DirectionalLight>();
    //m_directionalLight->direction = glm::vec3(0.5f, 0.7f, 1.0f);
    m_directionalLight->diffuseIntensity = glm::vec3(0.9f, 0.9f, 0.9f);
    m_scene->SetDirectionalLight(m_directionalLight);
#endif

    // create terrain entity
    CreateTerrainEntity();

    // create grass entity
    CreateGrassEntity(15000, 100.0f, 350.0f);

    // create house entity
    CreateHouseEntity(100.0f, 100.0f);

    // create hero entity
    CreateHeroEntity(100.0f, 150.0f);

    // create skydome / skybox entity
#ifdef SKYDOME
    CreateSkydomeEntity();
#else
    CreateSkyboxEntity();
#endif

    // init terrain entity components
    m_terrainEntity->Init();

    // todo
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ParticleShaderProgram>("particle");
    m_particleGenerator = std::make_shared<sg::ogl::particle::ParticleEmitter>(m_scene.get());
}

void GameState::GenerateGrassPositions(const float t_radius, const float t_offset, const int32_t t_instanceCount)
{
    srand(time(nullptr));
    rand();

    for (auto i{ 0 }; i < t_instanceCount; ++i)
    {
        sg::ogl::math::Transform transform;

        // 1. translation: displace along circle with radius in range [-offset, offset]
        const auto angle{ static_cast<float>(i) / static_cast<float>(t_instanceCount) * 360.0f };
        auto displacement{ static_cast<float>(rand() % static_cast<int>(2 * t_offset * 100)) / 100.0f - t_offset };
        const auto x{ sin(angle) * t_radius + displacement };
        displacement = static_cast<float>(rand() % static_cast<int>(2 * t_offset * 100)) / 100.0f - t_offset;
        const auto z{ cos(angle) * t_radius + displacement };
        transform.position = glm::vec3(x, m_terrain->GetHeightAtWorldPosition(x, z), z);
        transform.position.y += 5.0f;

        // 2. scale
        transform.scale = glm::vec3(4.0f);

        // 3. rotation
        transform.rotation = glm::vec3(180.0f, 0.0f, 0.0f);

        m_grassModelMatrices.push_back(transform.GetModelMatrix());
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::CreateSkydomeEntity()
{
    m_skydomeEntity = m_scene->CreateSkydomeEntity<DomeShaderProgram>("res/model/dome/dome.obj", "dome");
}

void GameState::CreateSkyboxEntity()
{
    const std::vector<std::string> textureFileNames
    {
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };

    m_skyboxEntity = m_scene->CreateSkyboxEntity<SkyboxShaderProgram>(textureFileNames, "skybox", 1500.0f);
}

void GameState::CreateTerrainEntity()
{
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(
        *GetApplicationContext()->GetTextureManager(),
        "res/config/Terrain.xml"
        );

    m_terrain->GenerateTerrain();

#ifdef DIRECTIONAL_LIGHTING
    m_terrainEntity = m_scene->CreateTerrainEntity<TerrainLightingShaderProgram, ComputeNormalmap, ComputeSplatmap>(m_terrain, "terrain_with_lighting");
#else
    m_terrainEntity = m_scene->CreateTerrainEntity<TerrainShaderProgram, ComputeNormalmap, ComputeSplatmap>(m_terrain, "terrain");
#endif
}

void GameState::CreateHouseEntity(const float t_worldX, const float t_worldZ)
{
#ifdef DIRECTIONAL_LIGHTING
    m_houseEntity = m_scene->CreateModelEntity<ModelLightingShaderProgram>("res/model/House/farmhouse_obj.obj", "model_with_lighting");
#else
    m_houseEntity = m_scene->CreateModelEntity<ModelShaderProgram>("res/model/House/farmhouse_obj.obj", "model");
#endif

    m_houseEntity->GetLocalTransform().position = glm::vec3(t_worldX, m_terrain->GetHeightAtWorldPosition(t_worldX, t_worldZ), t_worldZ);
}

void GameState::CreateHeroEntity(const float t_worldX, const float t_worldZ)
{
#ifdef DIRECTIONAL_LIGHTING
    m_heroEntity = m_scene->CreateModelEntity<ModelLightingShaderProgram>("res/model/panda/pandarenmale.obj", "model_with_lighting");
#else
    m_heroEntity = m_scene->CreateModelEntity<ModelShaderProgram>("res/model/panda/pandarenmale.obj", "model");
#endif

    m_heroEntity->GetLocalTransform().position = glm::vec3(t_worldX, m_terrain->GetHeightAtWorldPosition(t_worldX, t_worldZ), t_worldZ);
    m_heroEntity->GetLocalTransform().scale = glm::vec3(2.0f);
}

void GameState::CreateGrassEntity(const int32_t t_instanceCount, const float t_radius, const float t_offset)
{
#ifdef DIRECTIONAL_LIGHTING
    //m_grassEntity = m_scene->CreateModelEntity<InstancingLightingShaderProgram>("res/model/Grass_01/grassmodel.obj", "instancing_with_lighting");
    m_grassEntity = m_scene->CreateModelEntity<InstancingShaderProgram>("res/model/Grass_01/grassmodel.obj", "instancing");
#else
    m_grassEntity = m_scene->CreateModelEntity<InstancingShaderProgram>("res/model/Grass_01/grassmodel.obj", "instancing");
#endif

    m_grassEntity->instanceCount = t_instanceCount;
    GenerateGrassPositions(t_radius, t_offset, m_grassEntity->instanceCount);
    sg::ogl::scene::Scene::SetInstancePositions(m_grassModelMatrices, m_grassEntity);
}
