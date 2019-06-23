#include "GameState.h"

void GameState::Render()
{
    m_vao.BindVao();
    GetContext().shaderManager->GetShaderProgram("simple")->Bind();
    m_vao.DrawPrimitives();
    m_vao.UnbindVao();
}

bool GameState::Update(float t_dt)
{
    return true;
}

void GameState::Init()
{
    // copy to GPU
    m_vao.AllocateVertices(reinterpret_cast<float*>(m_vertices.data()), 3, 18 * sizeof(float), m_bufferLayout);
    m_vao.AllocateIndices(m_indices);

    // clear on CPU
    m_vertices.clear();
    m_indices.clear();

    // load shader
    GetContext().shaderManager->AddShaderProgram("simple");
}
