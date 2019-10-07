#include <random>
#include "GameState.h"
#include "DomeShaderProgram.h"
#include "ParticleShaderProgram.h"

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
    m_particleEmitter->Render();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void GameState::Init()
{
    // set clear color
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create particles emitter
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ParticleShaderProgram>("particle_anim");
    m_particleEmitter = std::make_shared<sg::ogl::particle::ParticleEmitter>(
        m_scene.get(),
        MAX_PARTICLES,
        "res/texture/particle/particleAtlas.png",
        4                                           // number of rows
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

            particle.position = glm::vec3(0.0f);
            particle.velocity = glm::vec3(velocityX(engine), 1.0f, velocityZ(engine));
            particle.scale = scale(engine);
            particle.lifetime = lifetime(engine);

            m_particleEmitter->AddParticle(particle);
        }
    }
}
