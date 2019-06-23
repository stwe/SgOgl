#pragma once

#include <glm/vec3.hpp>
#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 col;
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

    std::vector<Vertex> m_vertices{
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.8f, 0.2f, 0.8f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.3f, 0.8f) },
        { glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0.8f, 0.8f, 0.2f) }
    };

    std::vector<uint32_t> m_indices{ 0, 1, 2 };

    const sg::ogl::buffer::BufferLayout m_bufferLayout{
        { sg::ogl::buffer::VertexAttributeType::POSITION, "vPosition" },
        { sg::ogl::buffer::VertexAttributeType::COLOR, "vColor" }
    };

    void Init();
};
