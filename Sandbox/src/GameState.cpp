#include <random>
#include "GameState.h"
#include "shader/ParticleShaderProgram.h"
#include "shader/DomeShaderProgram.h"
#include "shader/SkyboxShaderProgram.h"
#include "shader/ModelShaderProgram.h"
#include "shader/GuiShaderProgram.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool GameState::Input()
{
    if (GetApplicationContext()->GetMouseInput()->IsRightButtonPressed())
    {
        m_scene->GetCurrentCamera().ProcessMouse(GetApplicationContext()->GetMouseInput()->GetDisplVec());
    }

    return true;
}

bool GameState::Update(const double t_dt)
{
    //m_particleEmitter->Update(t_dt);
    //BuildParticles();

    for (auto* entity : m_entities)
    {
        entity->Update();
    }

    m_waterTile->Update();

    //m_atmosphere.at("skydome")->Update();
    m_atmosphere.at("skybox")->Update();

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_W))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::FORWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_S))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::BACKWARD, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_A))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::LEFT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_D))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::RIGHT, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_O))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::UP, t_dt * CAMERA_VELOCITY);
    }

    if (GetApplicationContext()->GetWindow()->IsKeyPressed(GLFW_KEY_U))
    {
        m_scene->GetCurrentCamera().ProcessKeyboard(sg::ogl::camera::DOWN, t_dt * CAMERA_VELOCITY);
    }

    return true;
}

void GameState::Render()
{
    for (auto* entity : m_entities)
    {
        entity->Render();
    }

    m_waterTile->Render();

    //m_atmosphere.at("skydome")->Render();
    m_atmosphere.at("skybox")->Render();

    m_guiEntity->Render();

    //m_particleEmitter->Render(); // todo render before skybox
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    // enable face culling
    sg::ogl::OpenGl::EnableFaceCulling();

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(0.0f, 1.0f, -10.0f));

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ParticleShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ModelShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<SkyboxShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<DomeShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<GuiShaderProgram>();

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create scene loader
    m_sceneLoader = std::make_unique<sg::ogl::scene::SceneLoader>("res/config/Scene.xml");
    m_sceneLoader->LoadAtmosphere(m_scene.get(), m_atmosphere);
    m_sceneLoader->LoadMaterials(GetApplicationContext(), m_materials);
    m_sceneLoader->LoadEntities(m_scene.get(), m_entities, m_materials);

    // create water tile
    m_material = std::make_shared<sg::ogl::resource::Material>();
    m_material->kd = glm::vec3(0.0f, 0.0f, 1.0f);
    m_waterTile = m_scene->CreateModelEntity("res/model/plane/plane.obj", "model", m_material);
    m_waterTile->GetLocalTransform().position = glm::vec3(0.0f);
    m_waterTile->GetLocalTransform().scale = glm::vec3(60.0f, 1.0f, 60.0f);

    // create gui
    m_guiEntity = m_scene->CreateGuiEntity("res/texture/test.png", glm::vec2(0.5f), glm::vec2(0.25f), "gui");

    // create particles emitter
    /*
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ParticleShaderProgram>("particle_anim");
    m_particleEmitter = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        MAX_PARTICLES,
        "res/texture/particle/fire.png",
        8
        );

    BuildParticles();
    */
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::BuildParticles() const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> velocityX(-1.0f, 1.0f);
    const std::uniform_real_distribution<float> velocityZ(-1.0f, 1.0f);
    const std::uniform_real_distribution<float> scale(0.5f, 1.0f);
    const std::uniform_real_distribution<float> lifetime(2.0f, 4.0f);

    const auto nrOfparticles{ m_particleEmitter->GetParticles().size() };

    if (nrOfparticles < MAX_PARTICLES)
    {
        auto newParticles{ MAX_PARTICLES - nrOfparticles };
        if (newParticles > NEW_PARTICLES)
        {
            newParticles = NEW_PARTICLES;
        }

        //SG_OGL_LOG_DEBUG("[GameState::BuildParticles()] ...add {} new particles to existing {}.", newParticles, nrOfparticles);

        for (auto i{ 0u }; i < newParticles; ++i)
        {
            sg::ogl::particle::Particle particle;

            particle.position = glm::vec3(0.0f, 5.0f, 0.0f);
            particle.velocity = glm::vec3(velocityX(engine), 1.0f, velocityZ(engine));
            particle.scale = scale(engine);
            particle.lifetime = lifetime(engine);

            m_particleEmitter->AddParticle(particle);
        }
    }
}
