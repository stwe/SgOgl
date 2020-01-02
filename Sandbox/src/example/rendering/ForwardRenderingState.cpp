// This file is part of the SgOgl package.
// 
// Filename: ForwardRenderingState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "ForwardRenderingState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool ForwardRenderingState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool ForwardRenderingState::Update(const double t_dt)
{
    m_temp += static_cast<float>(t_dt);

    m_scene->GetCurrentCamera().Update(t_dt);

    auto& pointLights{ m_scene->GetPointLights() };
    for (auto& pointLight : pointLights)
    {
        pointLight->position.x += sinf(m_temp);
    }

    return true;
}

void ForwardRenderingState::Render()
{
    m_forwardRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void ForwardRenderingState::Init()
{
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(71.0f, 157.0f, -68.0f),
        -205.0f,
        -68.0f
    );
    m_firstPersonCamera->SetCameraVelocity(24.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    AddDirectionalLight();
    AddPointLights(4);

    m_forwardRenderSystem = std::make_unique<sg::ogl::ecs::system::ForwardRenderSystem>(m_scene.get());

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Plane/plane1.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(100.0f, 1.0f, 100.0f),
        false,
        false,
        false
    );
}

void ForwardRenderingState::AddDirectionalLight() const
{
    auto sun{ std::make_unique<sg::ogl::light::DirectionalLight>() };
    sun->direction = glm::vec3(0.9f, -0.1f, 0.0f);
    sun->diffuseIntensity = glm::vec3(0.3f, 0.2f, 0.1f);
    sun->specularIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
    m_scene->SetDirectionalLight(std::move(sun));
}

void ForwardRenderingState::AddPointLights(const int t_numPointLights) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> pos(-50.0f, 50.0f);
    const std::uniform_real_distribution<float> col(0.0f, 3.0f);

    for (auto i{ 0 }; i < t_numPointLights; ++i)
    {
        auto pointLight{ std::make_unique<sg::ogl::light::PointLight>() };
        pointLight->position = glm::vec3(pos(engine), 4.0f, pos(engine));
        pointLight->diffuseIntensity = glm::vec3(col(engine), col(engine), col(engine));
        pointLight->linear = 0.045f;
        pointLight->quadratic = 0.0075f;
        m_scene->AddPointLight(std::move(pointLight));
    }
}
