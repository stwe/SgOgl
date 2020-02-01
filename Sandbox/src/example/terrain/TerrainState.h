// This file is part of the SgOgl package.
// 
// Filename: TerrainState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

// todo: directional lighting
// todo: Fog
// todo: check number of specified root nodes -> 8 -> otherwise cracks will form
// todo: Triplanar UV mapping
// todo: The terrain has cracked since the GetHeightAt function was used.

class TerrainState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;

    using TerrainConfigSharedPtr = std::shared_ptr<sg::ogl::terrain::TerrainConfig>;
    using TerrainQuadtreeSharedPtr = std::shared_ptr<sg::ogl::terrain::TerrainQuadtree>;

    using TerrainQuadtreeRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>;
    using TerrainQuadtreeWfRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::TerrainQuadtreeWfRenderSystem>;

    using SkydomeRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SkydomeRenderSystem>;

    using SunRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::SunRenderSystem>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    TerrainState() = delete;

    explicit TerrainState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "TerrainState" }
    {
        Init();
    }

    TerrainState(const TerrainState& t_other) = delete;
    TerrainState(TerrainState&& t_other) noexcept = delete;
    TerrainState& operator=(const TerrainState& t_other) = delete;
    TerrainState& operator=(TerrainState&& t_other) noexcept = delete;

    ~TerrainState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[TerrainState::~TerrainState()] Destruct TerrainState.");
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

    TerrainConfigSharedPtr m_terrainConfig;
    TerrainQuadtreeSharedPtr m_terrainQuadtree;

    TerrainQuadtreeRenderSystemUniquePtr m_terrainQuadtreeRenderSystem;
    TerrainQuadtreeWfRenderSystemUniquePtr m_terrainQuadtreeWfRenderSystem;
    SkydomeRenderSystemUniquePtr m_skydomeRenderSystem;
    SunRenderSystemUniquePtr m_sunRenderSystem;

    bool m_renderWireframe{ false };

    std::shared_ptr<sg::ogl::light::Sun> m_sun;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // ImGui
    //-------------------------------------------------

    void InitImGui() const;
    void RenderImGui();
    static void CleanUpImGui();
};
