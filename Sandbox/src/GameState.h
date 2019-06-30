#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    //-------------------------------------------------
    // Vertex
    //-------------------------------------------------

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 col;
        glm::vec2 uv;
    };

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    GameState(sg::ogl::state::StateStack& t_stateStack, Context& t_context)
        : State{ t_stateStack, t_context }
    {
        Init();
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(float t_dt) override;
    void Render() override;

protected:

private:
    /**
     * @brief A Vertex Array Objects to store our vertex data.
     */
    sg::ogl::buffer::Vao m_vao;

    /**
     * @brief Store our texture Id.
     */
    uint32_t m_textureId{ 0 };

    /**
     * @brief A camera to get a view matrix (Camera space).
     */
    sg::ogl::camera::LookAtCamera m_camera;

    /**
     * @brief The perspective projection matrix (Projection space).
     */
    glm::mat4 m_projectionMatrix{ glm::mat4() };

    /**
     * @brief The model matrix (World space).
     */
    sg::ogl::math::Transform m_transformMatrix;

    /**
     * @brief Vertex data of our Triangle (Object space).
     */
    std::vector<Vertex> m_vertices{
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.8f, 0.2f, 0.8f), glm::vec2(0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.3f, 0.8f), glm::vec2(1.0f, 0.0f) },
        { glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0.8f, 0.8f, 0.2f), glm::vec2(0.5f, 1.0f) }
    };

    /**
     * @brief Enable indexing.
     */
    std::vector<uint32_t> m_indices{ 0, 1, 2 };

    /**
     * @brief Describes the buffer layout for the VBO.
     */
    const sg::ogl::buffer::BufferLayout m_bufferLayout{
        { sg::ogl::buffer::VertexAttributeType::POSITION, "aPosition" },
        { sg::ogl::buffer::VertexAttributeType::COLOR, "aColor" },
        { sg::ogl::buffer::VertexAttributeType::UV, "aUv" },
    };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    /**
     * @brief Performs all initializations.
     */
    void Init();
};
