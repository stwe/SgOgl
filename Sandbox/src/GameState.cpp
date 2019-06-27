#include "GameState.h"

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

bool GameState::Update(float t_dt)
{
    return true;
}

void GameState::Init()
{
    // load texture
    m_textureId = GetContext().textureManager->GetTextureIdFromPath("res/texture/grass.jpg");

    // load shader
    GetContext().shaderManager->AddShaderProgram("simple");

    // copy vertices to GPU
    m_vao.AllocateVertices(reinterpret_cast<float*>(m_vertices.data()), 3, 27 * sizeof(float), m_bufferLayout);
    m_vao.AllocateIndices(m_indices);

    // clear vertices on CPU
    m_vertices.clear();
    m_indices.clear();
}
