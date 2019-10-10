#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 16.0f };
    static constexpr auto MAX_PARTICLES{ 200 };
    static constexpr auto NEW_PARTICLES{ 80 };

    // scene graph
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // particles
    using ParticleEmitterSharedPtr = std::shared_ptr<sg::ogl::particle::ParticleEmitter>;

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

        delete m_skydomeEntity;
        delete m_skyboxEntity;
        delete m_planeEntity;
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;
    ParticleEmitterSharedPtr m_particleEmitter;

    std::shared_ptr<sg::ogl::resource::Material> m_planeMaterial;

    sg::ogl::scene::Entity* m_skydomeEntity{ nullptr };
    sg::ogl::scene::Entity* m_skyboxEntity{ nullptr };
    sg::ogl::scene::Entity* m_planeEntity{ nullptr };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void BuildParticles() const;
};
