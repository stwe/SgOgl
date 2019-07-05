#include "GameState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    return true;
}

bool GameState::Update(const float t_dt)
{
    const auto vel{ 0.1f };

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
    m_vao.BindVao();

    GetApplicationContext()->GetShaderManager()->GetShaderProgram("simple")->Bind();

    GetApplicationContext()->GetTextureManager()->BindForReading(m_textureId, GL_TEXTURE0);
    GetApplicationContext()->GetTextureManager()->UseBilinearFilter();
    GetApplicationContext()->GetTextureManager()->UseRepeatWrapping();

    const auto mvp{ m_projectionMatrix * m_camera.GetViewMatrix() * m_transformMatrix.GetModelMatrix() };
    GetApplicationContext()->GetShaderManager()->GetShaderProgram("simple")->SetUniform("transform", mvp);
    GetApplicationContext()->GetShaderManager()->GetShaderProgram("simple")->SetUniform("ourTexture", 0);

    m_vao.DrawPrimitives();

    m_vao.UnbindVao();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // load texture
#if defined(_WIN64) && defined(_MSC_VER)
    m_textureId = GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/grass.jpg");
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    m_textureId = GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("/home/steffen/Dev/SgOgl/Sandbox/res/texture/grass.jpg");
#else
    #error Unsupported platform or unsupported compiler!
#endif

    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram("simple");

    // copy vertices to GPU
    m_vao.AllocateVertices(reinterpret_cast<float*>(m_vertices.data()), 3, 27 * sizeof(float), m_bufferLayout);
    m_vao.AllocateIndices(m_indices);

    // we don't need the vertex data anymore
    m_vertices.clear();
    m_indices.clear();

    // get projection matrix
    m_projectionMatrix = GetApplicationContext()->GetWindow()->GetProjectionMatrix();

    // set Triangle world position
    m_transformMatrix.position = glm::vec3(0.0f, 0.0f, 1.0f);
}
