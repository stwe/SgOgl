#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 96.0f };

    // model and material - shared
    using ModelSharedPtr = std::shared_ptr<sg::ogl::resource::Model>;
    using MaterialSharedPtr = std::shared_ptr<sg::ogl::resource::Material>;

    // scene graph - unique
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // default model renderer - shared
    using RendererSharedPtr = std::shared_ptr<sg::ogl::scene::Renderer>;

    // terrain and terrain renderer - shared
    using TerrainSharedPtr = std::shared_ptr<sg::ogl::terrain::Terrain>;
    using TerrainRendererSharedPtr = std::shared_ptr<sg::ogl::scene::TerrainRenderer>;

    // camera - shared
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // skybox - shared
    using SkyboxSharedPtr = std::shared_ptr<sg::ogl::resource::Skybox>;
    using SkyboxRendererSharedPtr = std::shared_ptr<sg::ogl::scene::SkyboxRenderer>;

    // lighting - shared
    using DirectionalLightSharedPtr = std::shared_ptr<sg::ogl::light::DirectionalLight>;
    using PointLightSharedPtr = std::shared_ptr<sg::ogl::light::PointLight>;

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
    ModelSharedPtr m_skydomeModel;
    sg::ogl::scene::Node* m_skydomeNode{ nullptr };

    SceneUniquePtr m_scene;

    RendererSharedPtr m_renderer;

    TerrainSharedPtr m_terrain;
    TerrainRendererSharedPtr m_terrainRenderer;

    CameraSharedPtr m_camera;

    SkyboxSharedPtr m_skybox;
    SkyboxRendererSharedPtr m_skyboxRenderer;

    PointLightSharedPtr m_pointLight;
    DirectionalLightSharedPtr m_directionalLight;

    glm::mat4 m_projectionMatrix{ glm::mat4(1.0f) };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();
};
