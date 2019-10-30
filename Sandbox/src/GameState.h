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
#include "renderer/ModelRenderSystem.h"
#include "shader/ModelShaderProgram.h"
#include "shader/SkyboxShaderProgram.h"
#include "renderer/SkyboxRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 12.0f };

    // scene graph
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera
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
    entt::entity m_houseEntity;
    entt::entity m_skyboxEntity;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;
    std::unique_ptr<SkyboxRenderSystem<SkyboxShaderProgram>> m_skyboxRenderSystem;

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
    void CreateHouseEntity();
    void CreateSkyboxEntity();
};
