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

#include "shader/ModelShaderProgram.h"
#include "shader/InstancingShaderProgram.h"
#include "shader/ParticleShaderProgram.h"

#include "renderer/ModelRenderSystem.h"
#include "renderer/InstancingRenderSystem.h"
#include "renderer/ParticleRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 8.0f };
    static constexpr auto MAX_PARTICLES{ 400 };
    static constexpr auto NEW_PARTICLES{ 2 };

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
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    ParticleEmitterSharedPtr m_particleEmitter;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;
    std::unique_ptr<InstancingRenderSystem<InstancingShaderProgram>> m_instancingRenderSystem;
    std::unique_ptr<ParticleRenderSystem<ParticleShaderProgram>> m_particleRenderSystem;

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
    void BuildParticles() const;

    std::vector<glm::mat4> CreatePlantPositions(uint32_t t_instances) const;
};
