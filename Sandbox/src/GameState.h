#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    using ModelUniquePtr = std::unique_ptr<sg::ogl::resource::Model>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    explicit GameState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "GameState" }
    {
        Init();
    }

    ~GameState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[GameState::~GameState()] Destruct GameState.");
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
     * @brief A 3D Model.
     */
    ModelUniquePtr m_model;

    /**
     * @brief A camera to create and get a view matrix (Camera space).
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

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    /**
     * @brief Performs all initializations.
     */
    void Init();
};
