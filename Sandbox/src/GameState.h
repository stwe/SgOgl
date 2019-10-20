#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 32.0f };
    static constexpr auto WATER_HEIGHT{ 13.0f };

    // scene graph
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    // scene loader
    using SceneLoaderUniquePtr = std::unique_ptr<sg::ogl::scene::SceneLoader>;

    // camera
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    // atmosphere
    using AtmosphereKey = std::string;
    using AtmosphereContainer = std::map<AtmosphereKey, sg::ogl::scene::Entity*>;

    // materials
    using MaterialKey = std::string;
    using MaterialSharedPtr = std::shared_ptr<sg::ogl::resource::Material>;
    using MaterialContainer = std::map<MaterialKey, MaterialSharedPtr>;

    // entities
    using EntityContainer = std::vector<sg::ogl::scene::Entity*>;

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

        // todo delete entities && children
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
    SceneLoaderUniquePtr m_sceneLoader;

    AtmosphereContainer m_atmosphere;
    MaterialContainer m_materials;
    EntityContainer m_entities;

    CameraSharedPtr m_camera;

    // water
    sg::ogl::scene::Entity* m_waterTile{ nullptr };

    // guis
    sg::ogl::scene::Entity* m_guiReflection{ nullptr };
    sg::ogl::scene::Entity* m_guiRefraction{ nullptr };

    // fbo
    std::shared_ptr<sg::ogl::buffer::WaterFbos> m_waterFbos;

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void RenderReflectionTexture();
    void RenderRefractionTexture();
};
