#include "GameState.h"
#include "DomeShaderProgram.h"
#include "SkyboxShaderProgram.h"
#include "ModelShaderProgram.h"
#include "TerrainShaderProgram.h"
#include "TerrainLightingShaderProgram.h"
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

    return true;
}

void GameState::Render()
{
    m_terrainEntity->Render();
    m_houseEntity->Render();
    m_heroEntity->Render();

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
    m_directionalLight->diffuseIntensity = glm::vec3(0.8f, 0.8f, 0.8f);
    m_scene->SetDirectionalLight(m_directionalLight);
#endif

    // create a terrain
    CreateTerrainEntity();

    // create a house entity on position
    CreateHouseEntity();
    m_houseEntity->GetLocalTransform().position = glm::vec3(100.0f, 15.0f, 100.0f);

    // create a hero on position
    CreateHeroEntity();
    m_heroEntity->GetLocalTransform().position = glm::vec3(100.0f, 19.0f, 150.0f);
    m_heroEntity->GetLocalTransform().scale = glm::vec3(2.0f);

    // create skydome / skybox
#ifdef SKYDOME
    CreateSkydomeEntity();
#else
    CreateSkyboxEntity();
#endif

    // init terrain entity components
    m_terrainEntity->Init();
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

void GameState::CreateHouseEntity()
{
    m_houseEntity = m_scene->CreateModelEntity<ModelShaderProgram>("res/model/House/farmhouse_obj.obj", "model");
}

void GameState::CreateHeroEntity()
{
    m_heroEntity = m_scene->CreateModelEntity<ModelShaderProgram>("res/model/panda/pandarenmale.obj", "model");
}
