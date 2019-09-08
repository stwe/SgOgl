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
    // change current camera
    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_1))
    {
        m_scene->SetCurrentCamera(m_camera1);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_2))
    {
        m_scene->SetCurrentCamera(m_camera2);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_R))
    {
        m_stopRotation = !m_stopRotation;
    }

    if (!m_stopRotation)
    {
        // update nodes local transform
        m_sunNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.0315f, 0.0f);
        m_earthNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.125f, 0.0f);
        m_moonNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.125f, 0.0f);
    }

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
    // render sun, earth, moon and skybox
    m_scene->Render();

    // render asteroid field
    m_scene->Render(m_asteroidNode);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
     sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load all needed shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("skybox");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("light");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("model");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("instancing");

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // load models
    m_sphereModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/sphere/sphere2.obj");
    m_asteroidModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/rock/rock.obj");

    // create materials
    m_moonMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_earthMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_sunMaterial = std::make_shared<sg::ogl::resource::Material>();

    // load textures
    const auto moonId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/moon.jpg") };
    const auto earthId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/earth.jpg") };
    const auto sunId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/sun.jpg") };

    // config materials
    m_moonMaterial->mapKd = moonId;
    m_moonMaterial->ns = 32.0f;

    m_earthMaterial->mapKd = earthId;
    m_earthMaterial->ns = 32.0f;

    m_sunMaterial->mapKd = sunId;
    m_sunMaterial->ns = 32.0f;

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

    // create camera1
    m_camera1 = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera1->SetPosition(glm::vec3(0.0f, 0.0f, 6.0f));

    // create a 2nd camera
    m_camera2 = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera2->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));

    // create a point light
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(0.0f);
    m_pointLight->ambientIntensity = glm::vec3(0.1f);
    m_pointLight->diffuseIntensity = glm::vec3(0.9f, 0.9f, 0.7f);
    m_pointLight->specularIntensity = glm::vec3(1.0f);
    m_pointLight->linear = 0.0014f;
    m_pointLight->quadratic = 0.00004f;

    // create scene, pass the renderer
    m_scene = std::make_unique<sg::ogl::scene::Scene>(m_renderer, m_skyboxRenderer);

    // set camera1 as current
    m_scene->SetCurrentCamera(m_camera1);

    // set point light
    m_scene->SetPointLight(m_pointLight);

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

    // add the skybox to the scene
    m_scene->SetSkybox(m_skybox);

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

    m_asteroidNode = m_scene->CreateNode(m_asteroidModel);
    m_asteroidNode->SetDebugName("Asteroid");
    m_asteroidNode->instanceCount = 5000;

    // AddChild()
    //m_sunNode->AddChild(m_earthNode);
    //m_earthNode->AddChild(m_moonNode);

    // or SetParent()
    m_moonNode->SetParent(m_earthNode);
    m_earthNode->SetParent(m_sunNode);

    // add (only) the sun node to scene root node
    m_scene->AddNodeToRoot(m_sunNode);

    // create a model matrix for each asteroid instance
    GenerateAsteroidPositions(100.0f, 20.0f, m_asteroidNode->instanceCount);

    // create a vbo
    sg::ogl::scene::Scene::SetNodeInstancePositions(m_asteroidModelMatrices, m_asteroidNode);
}

void GameState::GenerateAsteroidPositions(const float t_radius, const float t_offset, const int32_t t_instanceCount)
{
    const auto time{ glfwGetTime() };
    srand(static_cast<unsigned>(time));

    for (auto i{ 0 }; i < t_instanceCount; ++i)
    {
        sg::ogl::math::Transform transform;

        // 1. translation: displace along circle with radius in range [-offset, offset]
        const auto angle{ static_cast<float>(i) / static_cast<float>(t_instanceCount) * 360.0f };
        auto displacement{ static_cast<float>(rand() % static_cast<int>(2 * t_offset * 100)) / 100.0f - t_offset };
        const auto x{ sin(angle) * t_radius + displacement };
        displacement = static_cast<float>(rand() % static_cast<int>(2 * t_offset * 100)) / 100.0f - t_offset;
        const auto y{ displacement * 0.4f };
        displacement = static_cast<float>(rand() % static_cast<int>(2 * t_offset * 100)) / 100.0f - t_offset;
        const auto z{ cos(angle) * t_radius + displacement };
        transform.position = glm::vec3(x, y, z);

        // 2. scale: scale between 0.05 and 0.25f
        const auto scale{ static_cast<float>(rand() % 20) / 100.0f + 0.05f };
        transform.scale = glm::vec3(scale);

        // 3. rotation
        const auto rotAngle{ static_cast<float>(rand() % 360) };
        transform.rotation.x = rotAngle * 0.4f;
        transform.rotation.y = rotAngle * 0.6f;
        transform.rotation.z = rotAngle * 0.8f;

        m_asteroidModelMatrices.push_back(transform.GetModelMatrix());
    }
}
