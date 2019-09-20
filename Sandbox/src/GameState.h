#pragma once

#include "SgOgl.h"

// render a Skydome instead a Skybox
#define SKYDOME

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 128.0f };

    // scene graph - unique
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // camera - shared
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // terrain - shared
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
    sg::ogl::scene::Entity* m_skydomeEntity{ nullptr };
    sg::ogl::scene::Entity* m_skyboxEntity{ nullptr };
    sg::ogl::scene::Entity* m_terrainEntity{ nullptr };

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;
    TerrainSharedPtr m_terrain;

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void CreateSkydomeEntity();
    void CreateSkyboxEntity();
    void CreateTerrainEntity();
};
