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
    const auto vel{ 0.5f };

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
    for (const auto& mesh : m_model->GetMeshes())
    {
        mesh->InitDraw();

        GetApplicationContext()->GetShaderManager()->GetShaderProgram("model")->Bind();

        GetApplicationContext()->GetTextureManager()->BindForReading(m_textureId, GL_TEXTURE0);
        GetApplicationContext()->GetTextureManager()->UseBilinearFilter();
        GetApplicationContext()->GetTextureManager()->UseRepeatWrapping();

        const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * m_transformMatrix.GetModelMatrix() };
        GetApplicationContext()->GetShaderManager()->GetShaderProgram("model")->SetUniform("transform", mvp);
        GetApplicationContext()->GetShaderManager()->GetShaderProgram("model")->SetUniform("ourTexture", 0);

        mesh->DrawPrimitives();

        mesh->EndDraw();
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load "model" shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("model");

    // load model && texture
#if defined(_WIN64) && defined(_MSC_VER)

    m_textureId = GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/grass.jpg");
    m_model = std::make_unique<sg::ogl::resource::Model>("res/model/nanosuit/nanosuit.obj", *GetApplicationContext()->GetTextureManager());

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    m_textureId = GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("/home/steffen/Dev/SgOgl/Sandbox/res/texture/grass.jpg");
    m_model = std::make_unique<sg::ogl::resource::Model>("/home/steffen/Dev/SgOgl/Sandbox/res/model/nanosuit/nanosuit.obj");

#else

    #error Unsupported platform or unsupported compiler!

#endif

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set a world position for the model
    m_transformMatrix.position = glm::vec3(0.0f, 0.0f, 1.0f);
}
