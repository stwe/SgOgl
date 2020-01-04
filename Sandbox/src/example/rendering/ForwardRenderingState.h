// This file is part of the SgOgl package.
// 
// Filename: ForwardRenderingState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class ForwardRenderingState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;
    using ForwardRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ForwardRenderSystem>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    ForwardRenderingState() = delete;

    explicit ForwardRenderingState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "ForwardRenderingState" }
    {
        Init();
    }

    ForwardRenderingState(const ForwardRenderingState& t_other) = delete;
    ForwardRenderingState(ForwardRenderingState&& t_other) noexcept = delete;
    ForwardRenderingState& operator=(const ForwardRenderingState& t_other) = delete;
    ForwardRenderingState& operator=(ForwardRenderingState&& t_other) noexcept = delete;

    ~ForwardRenderingState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[ForwardRenderingState::~ForwardRenderingState()] Destruct ForwardRenderingState.");
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
    FirstPersonCameraSharedPtr m_firstPersonCamera;
    ForwardRenderSystemUniquePtr m_forwardRenderSystem;

    float m_temp{ 0.0f };

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
    void AddDirectionalLight() const;
    void AddScenePointLights(int t_numPointLights = 4) const;
};
