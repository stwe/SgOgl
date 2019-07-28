#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    using RendererUniquePtr = std::unique_ptr<sg::ogl::scene::Renderer>;
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using ModelUniquePtr = std::unique_ptr<sg::ogl::resource::Model>;

    using SkyboxUniquePtr = std::unique_ptr<sg::ogl::resource::Skybox>;
    using SkyboxRendererUniquePtr = std::unique_ptr<sg::ogl::renderer::SkyboxRenderer>;
    using TerrainUniquePtr = std::unique_ptr<sg::ogl::terrain::Terrain>;
    using TerrainRendererUniquePtr = std::unique_ptr<sg::ogl::renderer::TerrainRenderer>;

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
    RendererUniquePtr m_renderer;
    SceneUniquePtr m_scene;
    ModelUniquePtr m_bushModel;
    ModelUniquePtr m_treeModel;

    /**
     * @brief Our Skybox.
     */
    SkyboxUniquePtr m_skybox;

    /**
     * @brief A renderer for the Skybox.
     */
    SkyboxRendererUniquePtr m_skyboxRenderer;

    /**
     * @brief Our terrain.
     */
    TerrainUniquePtr m_terrain;

    /**
     * @brief A Renderer for the terrain.
     */
    TerrainRendererUniquePtr m_terrainRenderer;

    /**
     * @brief A camera to create and get a view matrix (Camera space).
     */
    sg::ogl::camera::LookAtCamera m_camera;

    /**
     * @brief The perspective projection matrix (Projection space).
     */
    glm::mat4 m_projectionMatrix{ glm::mat4() };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    /**
     * @brief Performs all initializations.
     */
    void Init();
};
