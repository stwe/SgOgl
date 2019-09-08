#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    static constexpr auto CAMERA_VELOCITY{ 12.0f };

    using ModelSharedPtr = std::shared_ptr<sg::ogl::resource::Model>;
    using MaterialSharedPtr = std::shared_ptr<sg::ogl::resource::Material>;

    using RendererSharedPtr = std::shared_ptr<sg::ogl::scene::Renderer>;
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    using SkyboxSharedPtr = std::shared_ptr<sg::ogl::resource::Skybox>;
    using SkyboxRendererSharedPtr = std::shared_ptr<sg::ogl::scene::SkyboxRenderer>;

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
    glm::mat4 m_projectionMatrix{ glm::mat4() };

    ModelSharedPtr m_sphereModel;
    ModelSharedPtr m_asteroidModel;

    MaterialSharedPtr m_moonMaterial;
    MaterialSharedPtr m_earthMaterial;
    MaterialSharedPtr m_sunMaterial;

    sg::ogl::scene::Node* m_sunNode{ nullptr };
    sg::ogl::scene::Node* m_earthNode{ nullptr };
    sg::ogl::scene::Node* m_moonNode{ nullptr };
    sg::ogl::scene::Node* m_asteroidNode{ nullptr };

    std::vector<glm::mat4> m_asteroidModelMatrices;

    RendererSharedPtr m_renderer;
    CameraSharedPtr m_camera1;
    CameraSharedPtr m_camera2;
    SceneUniquePtr m_scene;

    SkyboxSharedPtr m_skybox;
    SkyboxRendererSharedPtr m_skyboxRenderer;

    PointLightSharedPtr m_pointLight;

    bool m_stopRotation{ false };

    //-------------------------------------------------
    // Init
    //-------------------------------------------------

    void Init();
    void GenerateAsteroidPositions(float t_radius, float t_offset, int32_t t_instanceCount);
};
