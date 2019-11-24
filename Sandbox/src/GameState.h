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
#include "shader/SkyboxShaderProgram.h"
#include "shader/DomeShaderProgram.h"
#include "shader/TerrainShaderProgram.h"
#include "shader/GuiShaderProgram.h"
#include "shader/InstancingShaderProgram.h"
#include "shader/WaterShaderProgram.h"

#include "renderer/ModelRenderSystem.h"
#include "renderer/SkyboxRenderSystem.h"
#include "renderer/SkydomeRenderSystem.h"
#include "renderer/TerrainRenderSystem.h"
#include "renderer/GuiRenderSystem.h"
#include "renderer/InstancingRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 128.0f };
    static constexpr auto WATER_HEIGHT{ 50.0f };

    // scene graph
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // terrain
    using TerrainSharedPtr = std::shared_ptr<sg::ogl::terrain::Terrain>;

    // water
    using WaterSharedPtr = std::shared_ptr<sg::ogl::water::Water>;

    // particle emitter
    using ParticleEmitterSharedPtr = std::shared_ptr<sg::ogl::particle::ParticleEmitter>;

    // mouse picker
    using MousePickerUniquePtr = std::unique_ptr<sg::ogl::input::MousePicker>;

    // directional light
    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;

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
    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;
    TerrainSharedPtr m_terrain;
    WaterSharedPtr m_water;
    MousePickerUniquePtr m_mousePicker;
    DirectionalLightSharedPtr m_sun;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;
    std::unique_ptr<SkyboxRenderSystem<SkyboxShaderProgram>> m_skyboxRenderSystem;
    std::unique_ptr<SkydomeRenderSystem<DomeShaderProgram>> m_skydomeRenderSystem;
    std::unique_ptr<TerrainRenderSystem<TerrainShaderProgram>> m_terrainRenderSystem;
    std::unique_ptr<GuiRenderSystem<GuiShaderProgram>> m_guiRenderSystem;
    std::unique_ptr<InstancingRenderSystem<InstancingShaderProgram>> m_instancingRenderSystem;
    std::unique_ptr<sg::ogl::ecs::system::WaterRenderSystem<WaterShaderProgram>> m_waterRenderSystem;

    glm::vec3 m_lampPosition{ glm::vec3(-965.0f, 56.0f, -2037.0f) };

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    std::vector<glm::mat4> CreatePlantPositions(uint32_t t_instances) const;
};
