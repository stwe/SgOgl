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
    // update nodes local transform
    m_sunNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.25f, 0.0f);
    m_earthNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.25f, 0.0f);
    m_moonNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.25f, 0.0f);

    // update scene - calc nodes world matrix
    m_scene->Update();

    // update camera
    const auto vel{ 16.0f };

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
    m_nanoModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/nanosuit/nanosuit.obj");

    // create renderer
    m_renderer = std::make_unique<sg::ogl::scene::Renderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_projectionMatrix
        );

    // create scene
    m_scene = std::make_unique<sg::ogl::scene::Scene>(*m_renderer, m_camera);

    // load textures
    const auto moonId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/moon.jpg") };
    const auto earthId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/earth.jpg") };
    const auto sunId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/sun.jpg") };

    // create materials
    m_moonMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_earthMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_sunMaterial = std::make_shared<sg::ogl::resource::Material>();

    m_moonMaterial->mapKd = moonId;
    m_earthMaterial->mapKd = earthId;
    m_sunMaterial->mapKd = sunId;

    // create scene nodes
    m_sunNode = m_scene->CreateNode(m_sphereModel, m_sunMaterial);
    m_sunNode->GetLocalTransform().position = glm::vec3(0.0f, 0.0f, 0.0f); // sun at center
    m_sunNode->GetLocalTransform().scale = glm::vec3(2.0f);
    m_sunNode->SetDebugName("Sun");

    m_earthNode = m_scene->CreateNode(m_sphereModel, m_earthMaterial);
    m_earthNode->GetLocalTransform().position = glm::vec3(10.0f, 0.0f, 0.0f); // earth 10 units from the sun
    m_earthNode->SetDebugName("Earth");

    m_moonNode = m_scene->CreateNode(m_sphereModel, m_moonMaterial);
    m_moonNode->GetLocalTransform().position = glm::vec3(2.0f, 0.0f, 0.0f); // moon 2 units from the earth
    m_moonNode->GetLocalTransform().scale = glm::vec3(0.25f);
    m_moonNode->SetDebugName("Moon");

    // AddChild()
    //m_sunNode->AddChild(m_earthNode);
    //m_earthNode->AddChild(m_moonNode);

    // or SetParent()
    m_moonNode->SetParent(m_earthNode);
    m_earthNode->SetParent(m_sunNode);

    // add the sun node to scene
    m_scene->AddNodeToRoot(m_sunNode);

    // add a Nanosuit with default material to the scene
    m_nano1Node = m_scene->CreateNode(m_nanoModel);
    m_nano1Node->GetLocalTransform().position = glm::vec3(30.0f, 0.0f, 0.0f);
    m_nano1Node->GetLocalTransform().scale = glm::vec3(0.25f);
    m_nano1Node->SetDebugName("Nano1");
    m_scene->AddNodeToRoot(m_nano1Node);

    // add an other Nanosuit to the scene
    m_nano2Node = m_scene->CreateNode(m_nanoModel, m_moonMaterial);
    m_nano2Node->GetLocalTransform().position = glm::vec3(60.0f, 0.0f, 0.0f);
    m_nano2Node->GetLocalTransform().scale = glm::vec3(0.25f);
    m_nano2Node->SetDebugName("Nano2");
    m_scene->AddNodeToRoot(m_nano2Node);

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
