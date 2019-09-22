#pragma once

#include "SgOgl.h"

// renders a skydome instead of a skybox
#define SKYDOME

// creates a directional light and uses the corresponding shaders
#define DIRECTIONAL_LIGHTING

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

    // directional light - shared
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
    sg::ogl::scene::Entity* m_skydomeEntity{ nullptr };
    sg::ogl::scene::Entity* m_skyboxEntity{ nullptr };
    sg::ogl::scene::Entity* m_terrainEntity{ nullptr };
    sg::ogl::scene::Entity* m_houseEntity{ nullptr };
    sg::ogl::scene::Entity* m_heroEntity{ nullptr };

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;
    TerrainSharedPtr m_terrain;

    DirectionalLightSharedPtr m_directionalLight;

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
    void CreateHouseEntity();
    void CreateHeroEntity();
};
