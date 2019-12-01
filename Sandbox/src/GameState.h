// This file is part of the SgOgl package.
// 
// Filename: GameState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 200.0f };

    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;
    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;
    using ParticleEmitterSharedPtr = std::shared_ptr<sg::ogl::particle::ParticleEmitter>;

    using ModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ModelRenderSystem>;
    using SkydomeRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkydomeRenderSystem>;
    using SkyboxRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkyboxRenderSystem>;
    using ParticleRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ParticleRenderSystem>;

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
    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;
    DirectionalLightSharedPtr m_sun;
    PointLightSharedPtr m_pointLight;
    ParticleEmitterSharedPtr m_particleEmitter;

    ModelRenderSystemUniquePtr m_modelRenderSystem;
    SkydomeRenderSystemUniquePtr m_skydomeRenderSystem;
    SkyboxRenderSystemUniquePtr m_skyboxRenderSystem;
    ParticleRenderSystemUniquePtr m_particleRenderSystem;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
