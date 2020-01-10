// This file is part of the SgOgl package.
// 
// Filename: WaterState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class WaterState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;
    using SkyboxRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkyboxRenderSystem>;
    using ForwardRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ForwardRenderSystem>;
    using WaterRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::WaterRenderSystem>;
    using SunRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SunRenderSystem>;

    using WaterSharedPtr = std::shared_ptr<sg::ogl::water::Water>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    WaterState() = delete;

    explicit WaterState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "WaterState" }
    {
        Init();
    }

    WaterState(const WaterState& t_other) = delete;
    WaterState(WaterState&& t_other) noexcept = delete;
    WaterState& operator=(const WaterState& t_other) = delete;
    WaterState& operator=(WaterState&& t_other) noexcept = delete;

    ~WaterState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[WaterState::~WaterState()] Destruct WaterState.");
        CleanUpImGui();
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

    SkyboxRenderSystemUniquePtr m_skyboxRenderSystem;
    ForwardRenderSystemUniquePtr m_forwardRenderSystem;
    WaterRenderSystemUniquePtr m_waterRenderSystem;
    SunRenderSystemUniquePtr m_sunRenderSystem;

    WaterSharedPtr m_water;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // ImGui
    //-------------------------------------------------

    void InitImGui() const;
    void RenderImGui() const;
    static void CleanUpImGui();
};
