// This file is part of the SgOgl package.
// 
// Filename: GameState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"
#include "shader/ModelShaderProgram.h"
#include "renderer/ModelRenderSystem.h"
#include "shader/SkyboxShaderProgram.h"
#include "renderer/SkyboxRenderSystem.h"
#include "shader/DomeShaderProgram.h"
#include "renderer/SkydomeRenderSystem.h"
#include "shader/TerrainShaderProgram.h"
#include "renderer/TerrainRenderSystem.h"
#include "shader/GuiShaderProgram.h"
#include "renderer/GuiRenderSystem.h"
#include "shader/InstancingShaderProgram.h"
#include "renderer/InstancingRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 24.0f };

    // scene graph
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // terrain
    using TerrainSharedPtr = std::shared_ptr<sg::ogl::terrain::Terrain>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    GameState() = delete;

    explicit GameState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "GameState" }
    {
        Init();
    }

    GameState(const GameState& t_other) = delete;
    GameState(GameState&& t_other) noexcept = delete;
    GameState& operator=(const GameState& t_other) = delete;
    GameState& operator=(GameState&& t_other) noexcept = delete;

    ~GameState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[GameState::~GameState()] Destruct GameState.");
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    TerrainSharedPtr m_terrain;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;
    std::unique_ptr<SkyboxRenderSystem<SkyboxShaderProgram>> m_skyboxRenderSystem;
    std::unique_ptr<SkydomeRenderSystem<DomeShaderProgram>> m_skydomeRenderSystem;
    std::unique_ptr<TerrainRenderSystem<TerrainShaderProgram>> m_terrainRenderSystem;
    std::unique_ptr<GuiRenderSystem<GuiShaderProgram>> m_guiRenderSystem;
    std::unique_ptr<InstancingRenderSystem<InstancingShaderProgram>> m_instancingRenderSystem;

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    void CreateHouseEntity();
    void CreateSkyboxEntity();
    void CreateSkydomeEntity();
    void CreateTerrainEntity();
    void CreateGuiEntity();

    void AddGrass(uint32_t t_instances, const std::string& t_path);
    void CreateGrassPositions(uint32_t t_instances, std::vector<glm::mat4>& t_matrices) const;
};
