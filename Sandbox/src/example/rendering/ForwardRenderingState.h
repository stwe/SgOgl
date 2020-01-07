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

    using SkyboxRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkyboxRenderSystem>;
    using ForwardRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ForwardRenderSystem>;
    using SunRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SunRenderSystem>;

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
