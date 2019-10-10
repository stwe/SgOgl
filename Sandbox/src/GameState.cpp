#include <random>
#include "GameState.h"
#include "ParticleShaderProgram.h"
#include "SkyboxShaderProgram.h"
#include "DomeShaderProgram.h"
#include "ModelShaderProgram.h"

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
    m_particleEmitter->Update(t_dt);
    BuildParticles();

    m_planeEntity->Update();

    //m_skydomeEntity->Update();
    m_skyboxEntity->Update();

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
    m_planeEntity->Render();

    //m_skydomeEntity->Render();
    m_skyboxEntity->Render();

    m_particleEmitter->Render(); // todo render before skybox
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    // enable face culling
    //sg::ogl::OpenGl::EnableFaceCulling();

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(0.0f, 1.0f, -10.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create plane entity
    m_planeMaterial = std::make_shared<sg::ogl::resource::Material>();
    const auto planeDiffTextureId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/test.png") };
    m_planeMaterial->mapKd = planeDiffTextureId;
    m_planeEntity = m_scene->CreateModelEntity<ModelShaderProgram>("res/model/plane/plane.obj", "model", m_planeMaterial);
    m_planeEntity->GetLocalTransform().scale = glm::vec3(100.0f, 1.0f, 100.0f);

    // create skydom entity
    m_skydomeEntity = m_scene->CreateSkydomeEntity<DomeShaderProgram>("res/model/dome/dome.obj", "dome");

    // create skybox entity
    const std::vector<std::string> textureFileNames
    {
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };

    m_skyboxEntity = m_scene->CreateSkyboxEntity<SkyboxShaderProgram>(textureFileNames, "skybox", 5000.0f);

    // create particles emitter
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ParticleShaderProgram>("particle_anim");
    m_particleEmitter = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        MAX_PARTICLES,
        "res/texture/particle/fire.png",
        8                                           // number of rows
        );

    // build particles
    BuildParticles();
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
