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

    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;

    using GuiRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::GuiRenderSystem>;
    using SkyboxRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkyboxRenderSystem>;
    using DeferredRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::DeferredRenderSystem>;
    using SunRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SunRenderSystem>;

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

    GuiRenderSystemUniquePtr m_guiRenderSystem;
    SkyboxRenderSystemUniquePtr m_skyboxRenderSystem;
    DeferredRenderSystemUniquePtr m_deferredRenderSystem;
    SunRenderSystemUniquePtr m_sunRenderSystem;

    float m_temp{ 0.0f };

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    /**
     * @brief Adds Point Lights to the Scene that are not an entity.
     * @param t_numPointLights Number of Point Lights.
     */
    void AddScenePointLights(int t_numPointLights = 4) const;

    /**
     * @brief Creates two entities from a 3D model and a Point Light.
     */
    void AddEntityPointLights() const;

    /**
     * @brief Creates the Sun from a texture and a Directional Light.
     * @param t_sunTexturePath The texture of the Sun.
     */
    void AddEntityDirectionalLight(const std::string& t_sunTexturePath) const;

    //-------------------------------------------------
    // ImGui
    //-------------------------------------------------

    void InitImGui() const;
    void RenderImGui() const;
    static void CleanUpImGui();
};
