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
#include "shader/ModelNormalmapShaderProgram.h"
#include "shader/DomeShaderProgram.h"

#include "renderer/ModelRenderSystem.h"
#include "renderer/ModelNormalmapRenderSystem.h"
#include "renderer/SkydomeRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 4.0f };

    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;
    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;

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

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;
    std::unique_ptr<ModelNormalmapRenderSystem<ModelNormalmapShaderProgram>> m_modelNormalmapRenderSystem;
    std::unique_ptr<SkydomeRenderSystem<DomeShaderProgram>> m_skydomeRenderSystem;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
