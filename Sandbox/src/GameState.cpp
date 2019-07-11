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

bool GameState::Update(const float t_dt)
{
    const auto vel{ 6.0f };

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
    // set world position for the model
    sg::ogl::math::Transform transform;
    transform.position = glm::vec3(400.0f, 10.0f, 400.0f);
    transform.scale = glm::vec3(4.0f);

    // render model
    m_modelRenderer->Render(*m_model, transform, "model");

    // render terrain
    m_terrainRenderer->Render(m_terrains, "terrain");
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("model");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("terrain");

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set the camera position
    m_camera.SetPosition(glm::vec3(200.0f, 60.0f, 200.0f));

    // load model && terrain
#if defined(_WIN64) && defined(_MSC_VER)

    m_model = std::make_unique<sg::ogl::resource::Model>("res/model/Tree_02/tree02.obj", *GetApplicationContext()->GetTextureManager());
    m_terrains.push_back(std::make_unique<sg::ogl::terrain::Terrain>(0.0f, 0.0f, *GetApplicationContext()->GetTextureManager(), "res/heightmap/heightmap.png"));

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    m_model = std::make_unique<sg::ogl::resource::Model>("/home/steffen/Dev/SgOgl/Sandbox/res/model/Tree_02/tree02.obj", *GetApplicationContext()->GetTextureManager());
    m_terrains.push_back(std::make_unique<sg::ogl::terrain::Terrain>(0.0f, 0.0f, *GetApplicationContext()->GetTextureManager(), "/home/steffen/Dev/SgOgl/Sandbox/res/heightmap/heightmap.png"));

#else

    #error Unsupported platform or unsupported compiler!

#endif

    // create model renderer
    m_modelRenderer = std::make_unique<sg::ogl::renderer::ModelRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );

    // create terrain renderer
    m_terrainRenderer = std::make_unique<sg::ogl::renderer::TerrainRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );
}
