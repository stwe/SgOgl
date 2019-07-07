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
    // get ShaderProgram
    auto& shaderProgram{ GetApplicationContext()->GetShaderManager()->GetShaderProgram("model") };

    // bind ShaderProgram
    shaderProgram->Bind();

    // set transform
    const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * m_transformMatrix.GetModelMatrix() };
    shaderProgram->SetUniform("transform", mvp);

    // set ambient intensity
    shaderProgram->SetUniform("ambientIntensity", glm::vec3(1.0f));

    // for each mesh
    for (const auto& mesh : m_model->GetMeshes())
    {
        auto& material{ mesh->GetMaterial() };

        shaderProgram->SetUniform("diffuseMap", 0);
        GetApplicationContext()->GetTextureManager()->BindForReading(material->mapKd, GL_TEXTURE0);

        mesh->InitDraw();
        mesh->DrawPrimitives();
        mesh->EndDraw();
    }

    shaderProgram->Unbind();
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

    m_model = std::make_unique<sg::ogl::resource::Model>("res/model/nanosuit/nanosuit.obj", *GetApplicationContext()->GetTextureManager());

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    m_model = std::make_unique<sg::ogl::resource::Model>("/home/steffen/Dev/SgOgl/Sandbox/res/model/nanosuit/nanosuit.obj");

#else

    #error Unsupported platform or unsupported compiler!

#endif

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set a world position for the model
    m_transformMatrix.position = glm::vec3(0.0f, 0.0f, 1.0f);
}
