// This file is part of the SgOgl package.
// 
// Filename: DeferredRenderingState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "DeferredRenderingState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool DeferredRenderingState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool DeferredRenderingState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    return true;
}

void DeferredRenderingState::Render()
{
    m_deferredRenderSystem->Render();

    //m_modelRenderSystem->Render();
    //m_guiRenderSystem->Render();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void DeferredRenderingState::Init()
{
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(-2.0f, 7.0f, -26.0f),
        87.0f,
        -10.0f
    );

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(0.9f, -0.1f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.3f, 0.2f, 0.1f);
    m_sun->specularIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
    m_scene->SetDirectionalLight(m_sun);

    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(0.0f, 8.0f, 0.0f);
    m_pointLight->diffuseIntensity = glm::vec3(1.0f, 0.77f, 0.56f);
    m_pointLight->linear = 0.045f;
    m_pointLight->quadratic = 0.0075f;
    m_scene->SetPointLight(m_pointLight);

    m_modelRenderSystem = std::make_unique<sg::ogl::ecs::system::ModelRenderSystem>(m_scene.get());
    m_guiRenderSystem = std::make_unique<sg::ogl::ecs::system::GuiRenderSystem>(m_scene.get());
    m_deferredRenderSystem = std::make_unique<sg::ogl::ecs::system::DeferredRenderSystem>(m_scene.get());

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Plane/plane1.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(8.0f, 1.0f, 8.0f),
        false,
        false,
        false,
        false
    );

    GetApplicationContext()->GetEntityFactory().CreateGuiEntity(-0.5f, 0.5f, 0.25f, 0.25f, m_deferredRenderSystem->GetFbo().GetPositionTextureId());
    GetApplicationContext()->GetEntityFactory().CreateGuiEntity(0.5f, 0.5f, 0.25f, 0.25f, m_deferredRenderSystem->GetFbo().GetAlbedoSpecTextureId());
    GetApplicationContext()->GetEntityFactory().CreateGuiEntity(-0.5f, -0.5f, 0.25f, 0.25f, m_deferredRenderSystem->GetFbo().GetNormalTextureId());
}
