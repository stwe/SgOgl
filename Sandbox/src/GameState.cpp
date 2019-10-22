#include <random>
#include "GameState.h"
#include "shader/SkyboxShaderProgram.h"
#include "shader/ModelShaderProgram.h"
#include "shader/GuiShaderProgram.h"
#include "shader/WaterShaderProgram.h"

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
    for (auto* entity : m_entities)
    {
        entity->Update(t_dt);
    }

    m_waterTile->Update(t_dt);

    m_atmosphere.at("skybox")->Update(t_dt);

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
    // enable clipping
    sg::ogl::OpenGl::EnableClipping();

    // render reflection texture
    RenderReflectionTexture();

    // render refraction texture
    RenderRefractionTexture();

    // render to screen
    sg::ogl::OpenGl::DisableClipping();
    //m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
    for (auto* entity : m_entities)
    {
        entity->Render();
    }

    m_waterTile->Render();

    m_atmosphere.at("skybox")->Render();

    //m_guiReflection->Render();
    //m_guiRefraction->Render();
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
    m_camera->SetPosition(glm::vec3(0.0f, 48.0f, -10.0f));

    // load shader
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<ModelShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<SkyboxShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<GuiShaderProgram>();
    GetApplicationContext()->GetShaderManager()->AddShaderProgram<WaterShaderProgram>();

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create and add the sun
    m_sun = std::make_shared<sg::ogl::light::DirectionalLight>();
    m_sun->direction = glm::vec3(1000.0f, -1000.0f, 1000.0f);
    m_sun->diffuseIntensity = glm::vec3(1.3f, 1.3f, 1.3f);
    m_scene->SetDirectionalLight(m_sun);

    // create scene loader and load configured scene entities
    m_sceneLoader = std::make_unique<sg::ogl::scene::SceneLoader>("res/config/Scene.xml");
    m_sceneLoader->LoadAtmosphere(m_scene.get(), m_atmosphere);
    m_sceneLoader->LoadMaterials(GetApplicationContext(), m_materials);
    m_sceneLoader->LoadEntities(m_scene.get(), m_entities, m_materials);

    // create fbos for water rendering
    m_waterFbos = std::make_shared<sg::ogl::buffer::WaterFbos>(GetApplicationContext());

    // create a water tile entity
    m_waterTile = m_scene->CreateModelEntity<sg::ogl::scene::component::WaterRenderConfig>("res/model/plane/plane.obj", "water");
    m_waterTile->GetLocalTransform().position = glm::vec3(22.0f, WATER_HEIGHT, 0.0f);
    m_waterTile->GetLocalTransform().scale = glm::vec3(30.0f, 1.0f, 30.0f);
    const auto dudvTextureId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/water/waterDUDV.png") };
    const auto normalTextureId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/water/normal.png") };
    m_scene->AddWaterComponent(
        m_waterTile,
        m_waterFbos->GetReflectionColorTextureId(),
        m_waterFbos->GetRefractionColorTextureId(),
        dudvTextureId,
        normalTextureId,
        m_waterFbos->GetRefractionDepthTextureId()
    );

    // create debug guis
    m_guiReflection = m_scene->CreateGuiEntity(m_waterFbos->GetReflectionColorTextureId(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.25f), "gui");
    m_guiRefraction = m_scene->CreateGuiEntity(m_waterFbos->GetRefractionColorTextureId(), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f), "gui");
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void GameState::RenderReflectionTexture()
{
    // render all above the water surface
    m_waterFbos->BindReflectionFboAsRenderTarget();

    const auto distance{ 2.0f * (m_camera->GetPosition().y - WATER_HEIGHT) };
    m_camera->GetPosition() -= distance;
    m_camera->InvertPitch();
    sg::ogl::OpenGl::ClearColorAndDepthBuffer();
    m_scene->SetCurrentClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -WATER_HEIGHT));
    for (auto* entity : m_entities)
    {
        entity->Render();
    }

    m_camera->GetPosition() += distance;
    m_camera->InvertPitch();

    m_waterFbos->UnbindRenderTarget();
}

void GameState::RenderRefractionTexture()
{
    // render all under the water surface
    m_waterFbos->BindRefractionFboAsRenderTarget();
    sg::ogl::OpenGl::ClearColorAndDepthBuffer();
    m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, WATER_HEIGHT));
    for (auto* entity : m_entities)
    {
        entity->Render();
    }

    m_waterFbos->UnbindRenderTarget();
}
