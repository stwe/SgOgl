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

class TerrainState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using FirstPersonCameraSharedPtr = std::shared_ptr<sg::ogl::camera::FirstPersonCamera>;

    using TerrainQuadtreeSharedPtr = std::shared_ptr<sg::ogl::terrain::TerrainQuadtree>;

    using TerrainQuadtreeRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>;
    using GuiRenderSystemUniquePtr = std::unique_ptr<sg::ogl::ecs::system::GuiRenderSystem>;

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

    TerrainQuadtreeSharedPtr m_terrainQuadtree;

    TerrainQuadtreeRenderSystemUniquePtr m_terrainQuadtreeRenderSystem;
    GuiRenderSystemUniquePtr m_guiRenderSystem;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
