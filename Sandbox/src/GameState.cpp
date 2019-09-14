#include "GameState.h"
#include "DomeShaderProgram.h"
#include "SkyboxShaderProgram.h"

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

    // add all needed shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<DomeShaderProgram>("dome");
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<SkyboxShaderProgram>("skybox");

    // load skydome model
    m_skydomeModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/dome/dome.obj");

    // init projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(370.0f, 100.0f, 370.0f));

    // create and setup scene
    m_scene = std::make_unique<sg::ogl::scene::Scene>();
    m_scene->SetCurrentCamera(m_camera);
    m_scene->projectionMatrix = m_projectionMatrix;

    // create skydome entity
    m_skydomeEntity = m_scene->CreateSkydomeEntity(
        m_skydomeModel,
        glm::vec3(GetApplicationContext()->GetProjectionOptions().farPlane * 0.5f),
        GetApplicationContext()->GetShaderManager()->GetShaderProgram("dome")
    );

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

    const auto cubemapId{ GetApplicationContext()->GetTextureManager()->GetCubemapId(textureFileNames) };

    // create skybox entity
    m_skyboxEntity = m_scene->CreateSkyboxEntity(
        cubemapId,
        GetApplicationContext()->GetShaderManager()->GetShaderProgram("skybox"),
        1500
    );
}
