#include "GameState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    if (GetApplicationContext()->GetMouseInput()->IsRightButtonPressed())
    {
        m_camera.ProcessMouse(GetApplicationContext()->GetMouseInput()->GetDisplVec());
    }

    return true;
}

bool GameState::Update(const double t_dt)
{
    const auto vel{ 32.0f };

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_W))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::FORWARD, t_dt * vel);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_S))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::BACKWARD, t_dt * vel);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_A))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::LEFT, t_dt * vel);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_D))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::RIGHT, t_dt * vel);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_O))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::UP, t_dt * vel);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_U))
    {
        m_camera.ProcessKeyboard(sg::ogl::camera::DOWN, t_dt * vel);
    }

    return true;
}

void GameState::Render()
{
    m_terrainRenderer->Render(*m_terrain, "terrain");
    //m_terrainRenderer->RenderNormals(*m_terrain, "normal", 1.0f);

    // render models


    // render skybox as last
    m_skyboxRenderer->Render(
        m_skybox->GetCubemapId(),
        *m_skybox->GetMesh(),
        "skybox"
    );
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
     sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("skybox");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("terrain");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("model");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("normal", true);

    // load compute shader
    GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("normalmap");
    GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("splatmap");

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set the camera position
    m_camera.SetPosition(glm::vec3(370.0f, 10.0f, 370.0f));

    ///////////////////////

	m_meshLoader = std::make_unique<sg::ogl::scene::MeshLoader>(*GetApplicationContext()->GetTextureManager());
	auto node = m_meshLoader->LoadMesh("res/model/nanosuit/nanosuit.obj");

	m_scene = std::make_unique<sg::ogl::scene::Scene>();
	m_scene->AddObject(node);

    //////////////////////////

    // skybox textures
    const std::vector<std::string> textureFileNames
    {
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };

    // create new skybox instance
    m_skybox = std::make_unique<sg::ogl::resource::Skybox>(*GetApplicationContext()->GetTextureManager(), textureFileNames);

    // create new skybox renderer instance
    m_skyboxRenderer = std::make_unique<sg::ogl::renderer::SkyboxRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );

    // create new terrain renderer instance
    m_terrainRenderer = std::make_unique<sg::ogl::renderer::TerrainRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );

    // create new terrain instance
    m_terrain = std::make_unique<sg::ogl::terrain::Terrain>(
        *GetApplicationContext()->GetTextureManager(),
        *GetApplicationContext()->GetShaderManager(),
        "res/config/Terrain.xml");

    // generate terrain
    m_terrain->GenerateTerrain();
}
