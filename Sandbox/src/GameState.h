#pragma once

#include "SgOgl.h"

#define SKYDOME

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 96.0f };

    // model and material - shared
    using ModelSharedPtr = std::shared_ptr<sg::ogl::resource::Model>;

    // scene graph - unique
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera - shared
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    GameState() = delete;

    explicit GameState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "GameState" }
    {
        Init();
    }

    GameState(const GameState& t_other) = delete;
    GameState(GameState&& t_other) noexcept = delete;
    GameState& operator=(const GameState& t_other) = delete;
    GameState& operator=(GameState&& t_other) noexcept = delete;

    ~GameState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[GameState::~GameState()] Destruct GameState.");
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    ModelSharedPtr m_skydomeModel;

    sg::ogl::scene::Entity* m_skydomeEntity{ nullptr };
    sg::ogl::scene::Entity* m_skyboxEntity{ nullptr };

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    glm::mat4 m_projectionMatrix{ glm::mat4(1.0f) };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();
};
