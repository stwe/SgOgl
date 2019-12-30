// This file is part of the SgOgl package.
// 
// Filename: DeferredRenderingState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class DeferredRenderingState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;
    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;

    using ModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ModelRenderSystem>;
    using GuiRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::GuiRenderSystem>;
    using DeferredRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::DeferredRenderSystem>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    DeferredRenderingState() = delete;

    explicit DeferredRenderingState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "DeferredRenderingState" }
    {
        Init();
    }

    DeferredRenderingState(const DeferredRenderingState& t_other) = delete;
    DeferredRenderingState(DeferredRenderingState&& t_other) noexcept = delete;
    DeferredRenderingState& operator=(const DeferredRenderingState& t_other) = delete;
    DeferredRenderingState& operator=(DeferredRenderingState&& t_other) noexcept = delete;

    ~DeferredRenderingState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[DeferredRenderingState::~DeferredRenderingState()] Destruct DeferredRenderingState.");
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

    DirectionalLightSharedPtr m_sun;
    PointLightSharedPtr m_pointLight;
    FirstPersonCameraSharedPtr m_firstPersonCamera;

    ModelRenderSystemUniquePtr m_modelRenderSystem;
    GuiRenderSystemUniquePtr m_guiRenderSystem;
    DeferredRenderSystemUniquePtr m_deferredRenderSystem;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
