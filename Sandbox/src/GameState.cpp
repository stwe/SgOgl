#include "GameState.h"

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
    // update scene - calc nodes world matrix
    m_scene->Update();

    // update current camera
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
    // render nodes
    m_scene->Render();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
     sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load all needed shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("dome");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("skybox");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("terrain");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("normal");

    // load compute shader
    GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("normalmap");
    GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("splatmap");

    // load skydome model
    m_skydomeModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/dome/dome.obj");

    // init projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // create renderer
    m_renderer = std::make_shared<sg::ogl::scene::Renderer>(
        *GetApplicationContext()->GetShaderManager(),
        m_projectionMatrix
        );

    // create skybox renderer
    m_skyboxRenderer = std::make_shared<sg::ogl::scene::SkyboxRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        m_projectionMatrix
        );

    // create terrain renderer
    m_terrainRenderer = std::make_shared<sg::ogl::scene::TerrainRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_projectionMatrix
        );

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(370.0f, 100.0f, 370.0f));

    // create a terrain
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(
        *GetApplicationContext()->GetTextureManager(),
        *GetApplicationContext()->GetShaderManager(),
        "res/config/Terrain.xml"
        );

    // generate terrain
    m_terrain->GenerateTerrain();

    // skybox textures
    const std::vector<std::string> textureFileNames
    {
        "res/texture/starfield/starfield_rt.jpg",
        "res/texture/starfield/starfield_lf.jpg",
        "res/texture/starfield/starfield_up.jpg",
        "res/texture/starfield/starfield_dn.jpg",
        "res/texture/starfield/starfield_bk.jpg",
        "res/texture/starfield/starfield_ft.jpg"
    };

    // create a skybox
    m_skybox = std::make_shared<sg::ogl::resource::Skybox>(*GetApplicationContext()->GetTextureManager(), textureFileNames);

    // create a directional light
    m_directionalLight = std::make_shared<sg::ogl::light::DirectionalLight>();

    // create a point light
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(0.0f);
    m_pointLight->ambientIntensity = glm::vec3(0.1f);
    m_pointLight->diffuseIntensity = glm::vec3(0.9f, 0.9f, 0.7f);
    m_pointLight->specularIntensity = glm::vec3(1.0f);

    // create and setup scene
    m_scene = std::make_unique<sg::ogl::scene::Scene>(m_renderer, m_skyboxRenderer, m_terrainRenderer);
    m_scene->SetCurrentCamera(m_camera);
    //m_scene->SetSkybox(m_skybox);
    m_scene->SetTerrain(m_terrain);
    m_scene->SetDirectionalLight(m_directionalLight);
    m_scene->SetPointLight(m_pointLight);

    // create scene nodes
    m_skydomeNode = m_scene->CreateSkydomeNode(m_skydomeModel, glm::vec3(GetApplicationContext()->GetProjectionOptions().farPlane * 0.5f));
    m_skydomeNode->SetDebugName("Skydome");

    m_scene->AddNodeToRoot(m_skydomeNode);
}
