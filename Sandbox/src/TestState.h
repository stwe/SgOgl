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
    static constexpr auto CAMERA_VELOCITY{ 4.0f };

    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;
    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;

    using SkeletalModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkeletalModelRenderSystem>;

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
        SG_OGL_LOG_DEBUG("[TestState::~TestState()] Destruct TestState.");
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
    std::shared_ptr<sg::ogl::resource::SkeletalModel> m_skeletalModel;
    SkeletalModelRenderSystemUniquePtr m_skeletalModelRenderSystem;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
