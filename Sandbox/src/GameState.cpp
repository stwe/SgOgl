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
    // update scene
    m_scene->Update();

    // update camera
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
    //m_terrainRenderer->Render(*m_terrain, "terrain");
    //m_terrainRenderer->RenderNormals(*m_terrain, "normal", 1.0f);

    // render scene
    m_scene->Render();

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
    //GetApplicationContext()->GetShaderManager()->AddShaderProgram("terrain");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("model");
    //GetApplicationContext()->GetShaderManager()->AddShaderProgram("normal", true);

    // load compute shader
    //GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("normalmap");
    //GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("splatmap");

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set the camera position
    m_camera.SetPosition(glm::vec3(0.0f, 0.0f, 6.0f));

    // load models
    m_sphereModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/sphere/sphere.obj");

    // create renderer
    m_renderer = std::make_unique<sg::ogl::scene::Renderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_projectionMatrix
        );

    // create scene
    m_scene = std::make_unique<sg::ogl::scene::Scene>(*m_renderer, m_camera);

    // load textures
    auto moon{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/moon.jpg") };
    auto earth{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/earth.jpg") };
    auto sun{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/sun.jpg") };

    // create materials
    auto* moonMaterial = new sg::ogl::resource::Material;
    auto* earthMaterial = new sg::ogl::resource::Material;
    auto* sunMaterial = new sg::ogl::resource::Material;

    moonMaterial->mapKd = moon;
    earthMaterial->mapKd = earth;
    sunMaterial->mapKd = sun;

    // create scene nodes
    auto* sunNode = m_scene->CreateNode(m_sphereModel, sunMaterial);
    sunNode->GetLocalTransform().position = glm::vec3(0.0f, 0.0f, 0.0f);
    sunNode->GetLocalTransform().scale = glm::vec3(2.0f);

    auto* earthNode = m_scene->CreateNode(m_sphereModel, earthMaterial);
    earthNode->GetLocalTransform().position = glm::vec3(10.0f, 0.0f, 0.0f);

    auto* moonNode = m_scene->CreateNode(m_sphereModel, moonMaterial);
    moonNode->GetLocalTransform().position = glm::vec3(4.0f, 0.0f, 0.0f);
    moonNode->GetLocalTransform().scale = glm::vec3(0.25f);

    // set children
    sunNode->AddChild(earthNode);
    earthNode->AddChild(moonNode);

    // add nodes to scene
    m_scene->AddNode(sunNode);

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
    /*
    m_terrainRenderer = std::make_unique<sg::ogl::renderer::TerrainRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );
    */

    // create new terrain instance
    /*
    m_terrain = std::make_unique<sg::ogl::terrain::Terrain>(
        *GetApplicationContext()->GetTextureManager(),
        *GetApplicationContext()->GetShaderManager(),
        "res/config/Terrain.xml");
    */

    // generate terrain
    //m_terrain->GenerateTerrain();
}
