#include "GameState.h"

bool GameState::Input()
{
    if (sg::ogl::input::MouseInput::GetInstance().IsRightButtonPressed())
    {
        SG_OGL_LOG_DEBUG("Rechte Maustaste");
        //m_sceneLoader->GetCameraManager()->GetLookAtCamera("default")->ProcessMouse(sg::ogl::input::MouseInput::GetInstance().GetDisplVec());
    }

    return true;
}

bool GameState::Update(float t_dt)
{
    return true;
}

void GameState::Render()
{
    m_vao.BindVao();

    GetContext().shaderManager->GetShaderProgram("simple")->Bind();

    GetContext().textureManager->BindForReading(m_textureId, GL_TEXTURE0);
    GetContext().textureManager->UseBilinearFilter();
    GetContext().textureManager->UseRepeatWrapping();

    GetContext().shaderManager->GetShaderProgram("simple")->SetUniform("ourTexture", 0);

    m_vao.DrawPrimitives();

    m_vao.UnbindVao();
}

void GameState::Init()
{
#if defined(_WIN64) && defined(_MSC_VER)

    // load texture
    m_textureId = GetContext().textureManager->GetTextureIdFromPath("res/texture/grass.jpg");

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    // load texture
    m_textureId = GetContext().textureManager->GetTextureIdFromPath("/home/steffen/Dev/SgOgl/Sandbox/res/texture/grass.jpg");

#else

#error Unsupported platform or unsupported compiler!

#endif

    // set clear color
    sg::ogl::Window::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // load shader
    GetContext().shaderManager->AddShaderProgram("simple");

    // copy vertices to GPU
    m_vao.AllocateVertices(reinterpret_cast<float*>(m_vertices.data()), 3, 27 * sizeof(float), m_bufferLayout);
    m_vao.AllocateIndices(m_indices);

    // clear vertices on CPU
    m_vertices.clear();
    m_indices.clear();
}