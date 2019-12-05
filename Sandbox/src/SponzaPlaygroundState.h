// This file is part of the SgOgl package.
// 
// Filename: SponzaPlaygroundState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class SponzaPlaygroundState : public sg::ogl::state::State
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

    SponzaPlaygroundState() = delete;

    explicit SponzaPlaygroundState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "SponzaPlaygroundState" }
    {
        Init();
    }

    SponzaPlaygroundState(const SponzaPlaygroundState& t_other) = delete;
    SponzaPlaygroundState(SponzaPlaygroundState&& t_other) noexcept = delete;
    SponzaPlaygroundState& operator=(const SponzaPlaygroundState& t_other) = delete;
    SponzaPlaygroundState& operator=(SponzaPlaygroundState&& t_other) noexcept = delete;

    ~SponzaPlaygroundState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[SponzaPlaygroundState::~SponzaPlaygroundState()] Destruct SponzaPlaygroundState.");

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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
