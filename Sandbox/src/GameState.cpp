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
    m_terrainRenderer->Render(m_terrains, "terrain");
    //m_terrainRenderer->RenderNormals(m_terrains, "normal", 1.0f);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("terrain");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("normal", true);

    // load compute shader
    GetApplicationContext()->GetShaderManager()->AddComputeShaderProgram("splatmap");

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set the camera position
    m_camera.SetPosition(glm::vec3(370.0f, 20.0f, 370.0f));

    // create terrain renderer
    m_terrainRenderer = std::make_unique<sg::ogl::renderer::TerrainRenderer>(
        *GetApplicationContext()->GetShaderManager(),
        *GetApplicationContext()->GetTextureManager(),
        m_camera,
        m_projectionMatrix
        );

    // set terrain textures
    sg::ogl::terrain::Terrain::TextureMap textureMap;
    textureMap.emplace("grass", "res/texture/grass0_DIF.jpg");
    textureMap.emplace("sand", "res/texture/Ground_11_DIF.jpg");
    textureMap.emplace("stone1", "res/texture/Ground_17_DIF.jpg");
    textureMap.emplace("stone2", "res/texture/Ground_21_DIF.jpg");

    // load terrain
    auto terrainUniquePtr{ std::make_unique<sg::ogl::terrain::Terrain>(
        0.0f,
        0.0f,
        *GetApplicationContext()->GetTextureManager(),
        *GetApplicationContext()->GetShaderManager(),
        "res/heightmap/heightmap.png",
        textureMap) };
    m_terrains.push_back(std::move(terrainUniquePtr));
}
