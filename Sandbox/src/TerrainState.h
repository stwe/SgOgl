// This file is part of the SgOgl package.
// 
// Filename: TerrainState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class TerrainState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using ThirdPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::ThirdPersonCamera>;

    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;

    using TerrainSharedPtr = std::shared_ptr<sg::ogl::terrain::Terrain>;

    using SkyboxRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkyboxRenderSystem>;
    using PlayerRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::PlayerRenderSystem>;
    using SkeletalModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkeletalModelRenderSystem>;
    using ModelRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::ForwardRenderSystem>;
    using TerrainRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::TerrainRenderSystem>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    TerrainState() = delete;

    explicit TerrainState(sg::ogl::state::StateStack* t_stateStack);

    TerrainState(const TerrainState& t_other) = delete;
    TerrainState(TerrainState&& t_other) noexcept = delete;
    TerrainState& operator=(const TerrainState& t_other) = delete;
    TerrainState& operator=(TerrainState&& t_other) noexcept = delete;

    ~TerrainState() noexcept override;

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    SceneUniquePtr m_scene;

    ThirdPersonCameraSharedPtr m_thirdPersonCamera;

    DirectionalLightSharedPtr m_sun;
    PointLightSharedPtr m_pointLight;

    TerrainSharedPtr m_terrain;

    SkyboxRenderSystemUniquePtr m_skyboxRenderSystem;
    PlayerRenderSystemUniquePtr m_playerRenderSystem;
    SkeletalModelRenderSystemUniquePtr m_skeletalModelRenderSystem;
    ModelRenderSystemUniquePtr m_modelRenderSystem;
    TerrainRenderSystemUniquePtr m_terrainRenderSystem;

    entt::entity m_player;

    uint32_t m_currentAnimation{ 0 };
    float m_ticksPerSecond{ 1200.0f };

    glm::vec3 m_playerPosition{ glm::vec3(-385.0f, 179.0f, -247.0f) };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // ImGui
    //-------------------------------------------------

    void InitImGui() const;
    void RenderImGui();
    static void CleanUpImGui();
};
