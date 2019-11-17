// This file is part of the SgOgl package.
// 
// Filename: GameState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "GameState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    if (GetApplicationContext()->GetMouseInput().IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput().GetDisplVec());
    }

    return true;
}

bool GameState::Update(const double t_dt)
{
    //m_particleEmitter->Update(t_dt);
    //BuildParticles();

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_W))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::FORWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_S))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::BACKWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_A))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::LEFT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_D))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::RIGHT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_O))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::UP, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_U))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::DOWN, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_P))
    {
        SG_OGL_LOG_INFO("Camera x: {}  y: {}  z: {}", m_scene->GetCurrentCamera().GetPosition().x, m_scene->GetCurrentCamera().GetPosition().y, m_scene->GetCurrentCamera().GetPosition().z);
        SG_OGL_LOG_INFO("Camera yaw: {}  pitch: {}", m_scene->GetCurrentCamera().GetYaw(), m_scene->GetCurrentCamera().GetPitch());
    }

    return true;
}

void GameState::Render()
{
    //m_modelRenderSystem->Render();
    m_instancingRenderSystem->Render();
    //m_particleEmitter->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(0.0f, 5.0f, -10.0f), 0.0f, -10.0f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create render systems
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
    m_instancingRenderSystem = std::make_unique<InstancingRenderSystem<InstancingShaderProgram>>(m_scene.get());
    //m_particleRenderSystem = std::make_unique<ParticleRenderSystem<ParticleShaderProgram>>(m_scene.get());

    // multiple grass instances
    const uint32_t instances{ 5000 };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        instances,
        "res/model/Grass/grassmodel.obj",
        CreatePlantPositions(instances)
    );

    // single grass
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Grass/grassmodel.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(64.0f)
    );

    //////////////////////////
    //////////////////////////

    /*
    m_particleEmitter = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        MAX_PARTICLES,
        "res/texture/particle/smoke.png",
        8 // number of rows
    );

    BuildParticles();
    */
}

void GameState::BuildParticles() const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> velocityX(-0.5f, 0.5f);
    const std::uniform_real_distribution<float> velocityZ(-0.5f, 0.5f);
    const std::uniform_real_distribution<float> scale(2.0f, 4.0f);
    const std::uniform_real_distribution<float> lifetime(1.0f, 4.0f);

    const auto nrOfparticles{ m_particleEmitter->GetParticles().size() };

    if (nrOfparticles < MAX_PARTICLES)
    {
        auto newParticles{ MAX_PARTICLES - nrOfparticles };
        if (newParticles > NEW_PARTICLES)
        {
            newParticles = NEW_PARTICLES;
        }

        for (auto i{ 0u }; i < newParticles; ++i)
        {
            sg::ogl::particle::Particle particle;

            particle.position = glm::vec3(0.0f, 20.0f, 0.0f);
            particle.velocity = glm::vec3(velocityX(engine), 1.0f, velocityZ(engine));
            particle.scale = scale(engine);
            particle.lifetime = lifetime(engine);

            m_particleEmitter->AddParticle(particle);
        }
    }
}

std::vector<glm::mat4> GameState::CreatePlantPositions(const uint32_t t_instances) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> posX(-100.0f , 100.0f);
    const std::uniform_real_distribution<float> posZ(-100.0f , 100.0);

    std::vector<glm::mat4> matrices;

    for (auto i{ 0u }; i < t_instances; ++i)
    {
        sg::ogl::math::Transform transform;

        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        //const auto height{ m_terrain->GetHeightAtWorldPosition(pos.x, pos.z) };

        transform.position = glm::vec3(pos.x, 0.0f, pos.z);
        transform.scale = glm::vec3(8.0f);

        matrices.push_back(transform.GetModelMatrix());
    }

    return matrices;
}
