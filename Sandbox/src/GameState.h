#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    using ModelUniquePtr = std::unique_ptr<sg::ogl::resource::Model>;
    using ModelRendererUniquePtr = std::unique_ptr<sg::ogl::renderer::ModelRenderer>;

    using TerrainUniquePtr = std::unique_ptr<sg::ogl::terrain::Terrain>;
    using TerrainRendererUniquePtr = std::unique_ptr<sg::ogl::renderer::TerrainRenderer>;
    using TerrainContainer = std::vector<TerrainUniquePtr>;

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
    bool Update(float t_dt) override;
    void Render() override;

protected:

private:
    /**
     * @brief A model.
     */
    ModelUniquePtr m_model;

    /**
     * @brief A Renderer for the model.
     */
    ModelRendererUniquePtr m_modelRenderer;

    /**
     * @brief Our terrains.
     */
    TerrainContainer m_terrains;

    /**
     * @brief A Renderer for the terrains.
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
