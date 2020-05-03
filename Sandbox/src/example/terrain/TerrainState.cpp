// This file is part of the SgOgl package.
// 
// Filename: TerrainState.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <random>
#include "TerrainState.h"
#include "ClientLog.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

TerrainState::TerrainState(sg::ogl::state::StateStack* t_stateStack)
    : State{ t_stateStack, "TerrainState" }
{
    Init();
}

TerrainState::~TerrainState() noexcept
{
    ClientLog::SG_SANDBOX_LOG_DEBUG("[TerrainState::~TerrainState()] Destruct TerrainState.");
    CleanUpImGui();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

bool TerrainState::Input()
{
    m_scene->GetCurrentCamera().Input();

    return true;
}

bool TerrainState::Update(const double t_dt)
{
    m_scene->GetCurrentCamera().Update(t_dt);

    if (m_renderWireframe)
    {
        m_terrainQuadtreeWfRenderSystem->Update(t_dt);
    }
    else
    {
        m_terrainQuadtreeRenderSystem->Update(t_dt);
    }

    return true;
}

void TerrainState::Render()
{
    if (m_renderWireframe)
    {
        m_terrainQuadtreeWfRenderSystem->Render();
    }
    else
    {
        m_terrainQuadtreeRenderSystem->Render();
    }

    //m_instancingRenderSystem->Render();

    m_skydomeRenderSystem->Render();
    m_sunRenderSystem->Render();

    RenderImGui();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void TerrainState::Init()
{
    InitImGui();

    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);

    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(1100.0f, 150.0f, 737.0f),
        89.0f,
        -2.0f
    );
    m_firstPersonCamera->SetCameraVelocity(256.0f);

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_firstPersonCamera);

    m_terrainConfig = std::make_shared<sg::ogl::terrain::TerrainConfig>(GetApplicationContext());
    m_terrainConfig->scaleXz = 8000.0f;
    m_terrainConfig->scaleY = 1700.0f;
    m_terrainConfig->rootNodes = 12;
    m_terrainConfig->normalStrength = 60.0f;
    m_terrainConfig->lodRanges = { 1750, 874, 386, 192, 100, 50, 0, 0 };
    m_terrainConfig->use16BitHeightmap = true;
    m_terrainConfig->InitMapsAndMorphing("res/heightmap/ruhpolding/Ruhpolding8km.png");
    m_terrainConfig->InitTextures(
        "res/terrain/terrain0/Grass (Hill).jpg",
        "res/terrain/terrain1/Grass.jpg",
        "res/terrain/terrain1/moss.jpg",
        "res/terrain/terrain0/Cliff (Layered Rock).jpg"
    );

    m_terrainQuadtree = std::make_shared<sg::ogl::terrain::TerrainQuadtree>(m_scene.get(), m_terrainConfig);
    GetApplicationContext()->GetEntityFactory().CreateTerrainQuadtreeEntity(m_terrainQuadtree);

    m_terrainQuadtreeRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeRenderSystem>(m_scene.get());
    m_terrainQuadtreeWfRenderSystem = std::make_unique<sg::ogl::ecs::system::TerrainQuadtreeWfRenderSystem>(m_scene.get());
    m_skydomeRenderSystem = std::make_unique<sg::ogl::ecs::system::SkydomeRenderSystem>(m_scene.get());
    m_sunRenderSystem = std::make_unique<sg::ogl::ecs::system::SunRenderSystem>(m_scene.get());
    m_instancingRenderSystem = std::make_unique<sg::ogl::ecs::system::InstancingRenderSystem>(m_scene.get());

    GetApplicationContext()->GetEntityFactory().CreateSkydomeEntity("res/model/Dome/dome.obj");

    m_sun = std::make_shared<sg::ogl::light::Sun>();
    m_sun->direction = glm::vec3(0.55f, -1.0f, -1.0f);
    m_sun->diffuseIntensity = glm::vec3(1.2f, 1.2f, 1.2f);
    m_sun->textureId = GetApplicationContext()->GetTextureManager().GetTextureIdFromPath("res/sun/sun.png");
    m_sun->scale = 12.0f;

    GetApplicationContext()->GetEntityFactory().CreateSunEntity(m_sun);

    m_scene->SetCurrentDirectionalLight(m_sun);

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        TREES,
        "res/model/Tree_01/billboardmodel.obj",
        CreateTreePositions(TREES),
        true
    );

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        TREES,
        "res/model/Tree_02/billboardmodel.obj",
        CreateTreePositions(TREES),
        true
    );

    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        GRASS,
        "res/model/Grass/grassmodel.obj",
        CreateGrassPositions(GRASS),
        true
    );
}

float TerrainState::GetHeightAt(const float t_x, const float t_z, const float t_min, const float t_max) const
{
    const auto heightmapWidth{ m_terrainConfig->GetHeightmapWidth() };

    auto pos{ glm::vec2(t_x, t_z) };
    pos -= m_terrainConfig->scaleXz * 0.5f;
    pos /= m_terrainConfig->scaleXz;

    const auto floorVec{ glm::vec2(static_cast<int>(floor(pos.x)), static_cast<int>(floor(pos.y))) };
    pos -= floorVec;
    pos *= heightmapWidth;

    const auto x0{ static_cast<int>(floor(pos.x)) };
    const auto x1{ x0 + 1 };
    const auto z0{ static_cast<int>(floor(pos.y)) };
    const auto z1{ z0 + 1 };

    const auto h0{ m_terrainConfig->GetHeightmapData()[heightmapWidth * z0 + x0] };
    const auto h1{ m_terrainConfig->GetHeightmapData()[heightmapWidth * z0 + x1] };
    const auto h2{ m_terrainConfig->GetHeightmapData()[heightmapWidth * z1 + x0] };
    const auto h3{ m_terrainConfig->GetHeightmapData()[heightmapWidth * z1 + x1] };

    const auto percentU{ pos.x - x0 };
    const auto percentV{ pos.y - z0 };

    float dU, dV;
    if (percentU > percentV)
    {   // bottom triangle
        dU = h1 - h0;
        dV = h3 - h1;
    }
    else
    {   // top triangle
        dU = h3 - h2;
        dV = h2 - h0;
    }

    auto h{ h0 + (dU * percentU) + (dV * percentV) };

    if (h < t_min || h > t_max)
    {
        return -900.0f;
    }

    h *= m_terrainConfig->scaleY;

    return h;
}

std::vector<glm::mat4> TerrainState::CreateTreePositions(const uint32_t t_instances)
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    std::uniform_real_distribution<float> posX(-3980.0f, 3980.0f);
    std::uniform_real_distribution<float> posZ(-3980.0f, 3980.0f);

    std::vector<glm::mat4> matrices;

    while (m_instances < t_instances)
    {
        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        const auto height{ GetHeightAt(pos.x, pos.z, 0.08f, 0.8f) };

        if (height > 0.0f)
        {
            sg::ogl::math::Transform transform;
            transform.position = glm::vec3(pos.x, height, pos.z);
            transform.scale = glm::vec3(36.0f);

            matrices.push_back(static_cast<glm::mat4>(transform));

            m_instances++;
        }
    }

    m_instances = 0;

    return matrices;
}

std::vector<glm::mat4> TerrainState::CreateGrassPositions(const uint32_t t_instances)
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    std::uniform_real_distribution<float> posX(-3980.0f, 3980.0f);
    std::uniform_real_distribution<float> posZ(-3980.0f, 3980.0f);

    std::vector<glm::mat4> matrices;

    while (m_instances < t_instances)
    {
        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        const auto height{ GetHeightAt(pos.x, pos.z, 0.01f, 0.1f) };

        if (height > 0.0f)
        {
            sg::ogl::math::Transform transform;
            transform.position = glm::vec3(pos.x, height, pos.z);
            transform.scale = glm::vec3(8.0f);

            matrices.push_back(static_cast<glm::mat4>(transform));

            m_instances++;
        }
    }

    m_instances = 0;

    return matrices;
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void TerrainState::InitImGui() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io{ ImGui::GetIO() };
    io.IniFilename = "res/config/Imgui.ini";

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(GetApplicationContext()->GetWindow().GetWindowHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void TerrainState::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::SliderFloat3("Camera", reinterpret_cast<float*>(&m_scene->GetCurrentCamera().GetPosition()), 0.0f, 6300.0f);

    ImGui::Separator();

    auto count{ 0 };
    m_terrainQuadtree->Count(count);
    ImGui::Text("Nodes count = %d", count);

    ImGui::Separator();

    ImGui::Checkbox("Morphing", &m_terrainConfig->morphingEnabled);
    ImGui::Checkbox("Tessellation", &m_terrainConfig->tessellationEnabled);
    ImGui::Checkbox("Wireframe", &m_renderWireframe);

    ImGui::Separator();

    ImGui::SliderFloat("Scale Y", &m_terrainConfig->scaleY, 1.0f, 2000.0f);

    ImGui::SliderInt("Tessellation Factor", &m_terrainConfig->tessellationFactor, 100, 1200);
    ImGui::SliderFloat("Tessellation Slope", &m_terrainConfig->tessellationSlope, 1.0f, 4.0f);
    ImGui::SliderFloat("Tessellation Shift", &m_terrainConfig->tessellationShift, -1.0f, 1.0f);

    ImGui::Separator();

    ImGui::SliderFloat3("Sun direction", reinterpret_cast<float*>(&m_scene->GetCurrentDirectionalLight().direction), -1.0f, 1.0f);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TerrainState::CleanUpImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
