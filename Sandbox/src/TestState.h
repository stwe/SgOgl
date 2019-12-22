// This file is part of the SgOgl package.
// 
// Filename: TestState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class TestState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using ThirdPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::ThirdPersonCamera>;
    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;

    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;

    using SkeletalModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkeletalModelRenderSystem>;
    using ModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ModelRenderSystem>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    TestState() = delete;

    explicit TestState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "TestState" }
    {
        Init();
    }

    TestState(const TestState& t_other) = delete;
    TestState(TestState&& t_other) noexcept = delete;
    TestState& operator=(const TestState& t_other) = delete;
    TestState& operator=(TestState&& t_other) noexcept = delete;

    ~TestState() noexcept override
    {
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
    ThirdPersonCameraSharedPtr m_thirdPersonCamera;

    DirectionalLightSharedPtr m_sun;
    PointLightSharedPtr m_pointLight;

    SkeletalModelRenderSystemUniquePtr m_skeletalModelRenderSystem;
    ModelRenderSystemUniquePtr m_modelRenderSystem;

    entt::entity m_castleGuardIdle;
    entt::entity m_player;

    uint32_t m_currentAnimation{ 0 };
    float m_ticksPerSecond{ 1200.0f };

    glm::vec3 m_playerPos{ glm::vec3(10.0f, 10.0f, -2.0f) };
    glm::vec3 m_playerRot{ glm::vec3(0.0f) };

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
    void InitImGui() const;
    void RenderImGui();
    static void CleanUpImGui();
};
