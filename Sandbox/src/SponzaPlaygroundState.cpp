// This file is part of the SgOgl package.
// 
// Filename: SponzaPlaygroundState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include <glm/gtc/type_ptr.hpp>
#include "SponzaPlaygroundState.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool SponzaPlaygroundState::Input()
{
    if (GetApplicationContext()->GetMouseInput().IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput().GetDisplVec());
    }

    return true;
}

bool SponzaPlaygroundState::Update(const double t_dt)
{
    m_particleRenderSystem->Update(t_dt);

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

    // move the lamp model
    auto view = m_scene->GetApplicationContext()->registry.view<
        sg::ogl::ecs::component::MoveableComponent,
        sg::ogl::ecs::component::ModelComponent,
        sg::ogl::ecs::component::TransformComponent>();

    for (auto entity : view)
    {
        if (GetApplicationContext()->GetMouseInput().IsLeftButtonPressed())
        {
            auto& transformComponent = view.get<sg::ogl::ecs::component::TransformComponent>(entity);
            transformComponent.position = m_pointLight->position;

            m_pointLight->position = transformComponent.position;
        }
    }

    return true;
}

void SponzaPlaygroundState::Render()
{
    // feed inputs to ImGui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_modelRenderSystem->Render();
    m_skeletalModelRenderSystem->Render();
    //m_skydomeRenderSystem->Render();
    m_particleRenderSystem->Render(),
    m_skyboxRenderSystem->Render();

    // render your GUI
    ImGui::Begin("Sponza Playground");
    ImGui::SliderFloat3("Directional Light Direction", value_ptr(m_sun->direction), -1.0, 1.0);
    ImGui::SliderFloat3("Point Light Position", value_ptr(m_pointLight->position), -1000.0, 1000.0);
    ImGui::SliderFloat("Point Light Linear", &m_pointLight->linear, 0.001, 0.9);
    ImGui::SliderFloat("Point Light Quadratic", &m_pointLight->quadratic, 0.000007, 1.9);
    ImGui::End();

    // render ImGui to the screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void SponzaPlaygroundState::Init()
{
    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // setup ImGui style
    ImGui::StyleColorsDark();

    // setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::CornflowerBlue());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(1197.0f, 171.0f, -23.0f), -179.0f, 1.0f);
    m_camera->SetMouseSensitivity(0.3f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // set ambient
    m_scene->SetAmbientIntensity(glm::vec3(0.2f));

    // create and add the sun to the scene
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(0.9f, -0.1f, 0.0f);
    m_sun->diffuseIntensity = glm::vec3(0.3f, 0.2f, 0.1f);
    m_sun->specularIntensity = glm::vec3(0.2f, 0.2f, 0.2f);
    m_scene->SetDirectionalLight(m_sun);

    // create and add a point light to the scene
    m_pointLight = std::make_shared<sg::ogl::light::PointLight>();
    m_pointLight->position = glm::vec3(91.0f, 165.0f, -42.0f);
    m_pointLight->diffuseIntensity = glm::vec3(1.0f, 0.57f, 10.16f);
    m_pointLight->linear = 0.007f;
    m_pointLight->quadratic = 0.0002f;
    m_scene->SetPointLight(m_pointLight);

    // create render systems
    m_modelRenderSystem = std::make_unique<sg::ogl::ecs::system::ModelRenderSystem>(m_scene.get());
    m_skeletalModelRenderSystem = std::make_unique<sg::ogl::ecs::system::SkeletalModelRenderSystem>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<sg::ogl::ecs::system::SkydomeRenderSystem>(m_scene.get());
    m_skyboxRenderSystem = std::make_unique<sg::ogl::ecs::system::SkyboxRenderSystem>(m_scene.get());
    m_particleRenderSystem = std::make_unique<sg::ogl::ecs::system::ParticleRenderSystem>(m_scene.get());

    ////////////////// Create Entities //////////////////

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Sponza/sponza.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        false,
        false,
        true,
        false
    );

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Streetlamp/streetlamp.obj",
        glm::vec3(91.0f, 0.0f, -42.0f),
        glm::vec3(0.0f),
        glm::vec3(16.0f),
        false,
        false,
        false,
        true
    );

    GetApplicationContext()->GetEntityFactory().CreateSkeletalModelEntity(
        "res/model/CastleGuard01/Idle.dae",
        glm::vec3(40.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -180.0f, 0.0f),
        glm::vec3(0.5f),
        false,
        false,
        false,
        false
    );

    /*
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };
    */
    const std::vector<std::string> cubemapFileNames{
        "res/texture/starfield/starfield_rt.jpg",
        "res/texture/starfield/starfield_lf.jpg",
        "res/texture/starfield/starfield_up.jpg",
        "res/texture/starfield/starfield_dn.jpg",
        "res/texture/starfield/starfield_bk.jpg",
        "res/texture/starfield/starfield_ft.jpg"
    };
    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);
    //GetApplicationContext()->GetEntityFactory().CreateSkydomeEntity("res/model/Dome/dome.obj");

    sg::ogl::particle::BuildConfig buildConfig;
    buildConfig.xRange.x = -2.0f;
    buildConfig.xRange.y = 2.0f;

    buildConfig.yRange.x = 6.0f;
    buildConfig.yRange.y = 16.0f;

    buildConfig.zRange.x = -2.0f;
    buildConfig.zRange.y = 2.0f;

    buildConfig.scaleRange.x = 1.0f;
    buildConfig.scaleRange.y = 2.0f;

    buildConfig.lifetimeRange.x = 1.0f;
    buildConfig.lifetimeRange.y = 4.0f;

    // create fire entity (instancing - particle system)
    m_particleEmitter = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        glm::vec3(490.0f, 125.0f, -220.0f),     // root position
        800,                                             // max particles
        2,                                               // new particles
        "res/texture/particle/explo.png",
        4                                                // number of rows
    );
    m_particleEmitter->SetGravityEffect(0.3f);
    m_particleEmitter->SetBuildConfig(buildConfig);
    GetApplicationContext()->GetEntityFactory().CreateParticleEntity(m_particleEmitter);
}
