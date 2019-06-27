#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 col;
        glm::vec2 uv;
    };

    GameState(sg::ogl::state::StateStack& t_stateStack, Context& t_context)
        : State{ t_stateStack, t_context }
    {
        Init();
    }

    void Render() override;
    bool Update(float t_dt) override;

protected:

private:
    sg::ogl::buffer::Vao m_vao;
    uint32_t m_textureId{ 0 };

    std::vector<Vertex> m_vertices{
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.8f, 0.2f, 0.8f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.3f, 0.8f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0.8f, 0.8f, 0.2f), glm::vec2(0.5f, 1.0f) }
    };

    std::vector<uint32_t> m_indices{ 0, 1, 2 };

    const sg::ogl::buffer::BufferLayout m_bufferLayout{
        { sg::ogl::buffer::VertexAttributeType::POSITION, "aPosition" },
        { sg::ogl::buffer::VertexAttributeType::COLOR, "aColor" },
        { sg::ogl::buffer::VertexAttributeType::UV, "aUv" },
    };

    void Init();
};
