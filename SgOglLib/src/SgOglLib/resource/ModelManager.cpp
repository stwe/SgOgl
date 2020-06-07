// This file is part of the SgOgl package.
// 
// Filename: ModelManager.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "ModelManager.h"
#include "Model.h"
#include "SkeletalModel.h"
#include "Mesh.h"
#include "Application.h"
#include "SgOglException.h"
#include "Core.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ModelManager::ModelManager(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[ModelManager::ModelManager()] Null pointer.");
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::ModelManager()] Create ModelManager.");

    AddMaterials();
    AddStaticMeshes();
}

sg::ogl::resource::ModelManager::~ModelManager() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::~ModelManager()] Destruct ModelManager.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::resource::Material sg::ogl::resource::ModelManager::GetMaterialByName(const std::string& t_name)
{
    if (m_materials.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::GetMaterialByName()] The Material could not be found.");
    }

    return m_materials.at(t_name);
}

sg::ogl::resource::ModelManager::StaticMeshSharedPtr sg::ogl::resource::ModelManager::GetStaticMeshByName(const std::string& t_name)
{
    if (m_staticMeshes.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::GetStaticMeshByName()] The Mesh could not be found.");
    }

    return m_staticMeshes.at(t_name);
}

sg::ogl::resource::ModelManager::ModelSharedPtr sg::ogl::resource::ModelManager::GetModel(const std::string& t_fullFilePath)
{
    return GetModelWithFlags(t_fullFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
}

sg::ogl::resource::ModelManager::ModelSharedPtr sg::ogl::resource::ModelManager::GetModelWithFlags(const std::string& t_fullFilePath, const unsigned int t_pFlags)
{
    // load the model if it does not already exist
    if (m_models.count(t_fullFilePath) == 0)
    {
        auto modelSharedPtr{ std::make_shared<Model>(t_fullFilePath, m_application, t_pFlags) };
        SG_OGL_CORE_ASSERT(modelSharedPtr, "[ModelManager::GetModelByPath()] Null pointer.");

        m_models.emplace(t_fullFilePath, modelSharedPtr);
    }

    return m_models.at(t_fullFilePath);
}

sg::ogl::resource::ModelManager::SkeletalModelSharedPtr sg::ogl::resource::ModelManager::GetSkeletalModel(const std::string& t_fullFilePath, unsigned int t_pFlags)
{
    // load the skeletal model if it does not already exist
    if (m_skeletalModels.count(t_fullFilePath) == 0)
    {
        auto skeletalModelSharedPtr{ std::make_shared<SkeletalModel>(t_fullFilePath, m_application, t_pFlags) };
        SG_OGL_CORE_ASSERT(skeletalModelSharedPtr, "[ModelManager::GetSkeletalModelByPath()] Null pointer.");

        m_skeletalModels.emplace(t_fullFilePath, skeletalModelSharedPtr);
    }

    return m_skeletalModels.at(t_fullFilePath);
}

sg::ogl::resource::ModelManager::VertexContainer sg::ogl::resource::ModelManager::GetParticleVertices()
{
    VertexContainer vertices{
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f
    };

    return vertices;
}

//-------------------------------------------------
// Add
//-------------------------------------------------

void sg::ogl::resource::ModelManager::AddMaterial(const Material& t_material)
{
    if (m_materials.count(t_material.newmtl) > 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::AddMaterial()] The Material already exists.");
    }

    m_materials.emplace(t_material.newmtl, t_material);
}

void sg::ogl::resource::ModelManager::AddStaticMesh(const StaticMeshSharedPtr& t_staticMesh)
{
    if (m_staticMeshes.count(t_staticMesh->GetName()) > 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::AddStaticMesh()] The Mesh already exists.");
    }

    m_staticMeshes.emplace(t_staticMesh->GetName(), t_staticMesh);
}

//-------------------------------------------------
// Built in
//-------------------------------------------------

void sg::ogl::resource::ModelManager::AddMaterials()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add emerald material.");

    Material emerald;
    emerald.newmtl = "emerald";
    emerald.ka = { glm::vec3(0.0215f, 0.1745f, 0.0215f) };
    emerald.kd = { glm::vec3(0.07568f, 0.61424f, 0.07568f) };
    emerald.ks = { glm::vec3(0.633f, 0.727811f, 0.633f) };
    emerald.ns = 0.6f;
    m_materials.emplace(emerald.newmtl, emerald);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add jade material.");

    Material jade;
    jade.newmtl = "jade";
    jade.ka = { glm::vec3(0.135f, 0.2225f, 0.0215f) };
    jade.kd = { glm::vec3(0.54f, 0.89f, 0.63f) };
    jade.ks = { glm::vec3(0.316228f) };
    jade.ns = 0.1f;
    m_materials.emplace(jade.newmtl, jade);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add obsidian material.");

    Material obsidian;
    obsidian.newmtl = "obsidian";
    obsidian.ka = { glm::vec3(0.05375f, 0.05f, 0.06625f) };
    obsidian.kd = { glm::vec3(0.18275f, 0.17f, 0.22525f) };
    obsidian.ks = { glm::vec3(0.332741f, 0.328634f, 0.346435f) };
    obsidian.ns = 0.3f;
    m_materials.emplace(obsidian.newmtl, obsidian);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add pearl material.");

    Material pearl;
    pearl.newmtl = "pearl";
    pearl.ka = { glm::vec3(0.25f, 0.20725f, 0.20725f) };
    pearl.kd = { glm::vec3(1.0f, 0.829f, 0.829f) };
    pearl.ks = { glm::vec3(0.296648f) };
    pearl.ns = 0.088f;
    m_materials.emplace(pearl.newmtl, pearl);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add gold material.");

    Material gold;
    gold.newmtl = "gold";
    gold.ka = { glm::vec3(0.24725f, 0.1995f, 0.0745f) };
    gold.kd = { glm::vec3(0.75164f, 0.60648f, 0.22648f) };
    gold.ks = { glm::vec3(0.628281f, 0.555802f, 0.366065f) };
    gold.ns = 0.4f;
    m_materials.emplace(gold.newmtl, gold);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add silver material.");

    Material silver;
    silver.newmtl = "silver";
    silver.ka = { glm::vec3(0.19225f) };
    silver.kd = { glm::vec3(0.50754f) };
    silver.ks = { glm::vec3(0.508273f) };
    silver.ns = 0.4f;
    m_materials.emplace(silver.newmtl, silver);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add bronze material.");

    Material bronze;
    bronze.newmtl = "bronze";
    bronze.ka = { glm::vec3(0.2125f, 0.1275f, 0.054f) };
    bronze.kd = { glm::vec3(0.714f, 0.4284f, 0.18144f) };
    bronze.ks = { glm::vec3(0.393548f, 0.271906f, 0.166721f) };
    bronze.ns = 0.2f;
    m_materials.emplace(bronze.newmtl, bronze);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add cyan plastic material.");

    Material cyanPlastic;
    cyanPlastic.newmtl = "cyan_plastic";
    cyanPlastic.ka = { glm::vec3(0.0f, 0.1f, 0.06f) };
    cyanPlastic.kd = { glm::vec3(0.0f, 0.50980392f, 0.50980392f) };
    cyanPlastic.ks = { glm::vec3(0.50196078f) };
    cyanPlastic.ns = 0.25f;
    m_materials.emplace(cyanPlastic.newmtl, cyanPlastic);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add red rubber material.");

    Material redRubber;
    redRubber.newmtl = "red_rubber";
    redRubber.ka = { glm::vec3(0.05f, 0.0f, 0.0f) };
    redRubber.kd = { glm::vec3(0.5f, 0.4f, 0.4f) };
    redRubber.ks = { glm::vec3(0.7f, 0.04f, 0.04f) };
    redRubber.ns = 0.078125f;
    m_materials.emplace(redRubber.newmtl, redRubber);

    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add chrome material.");

    Material chrome;
    chrome.newmtl = "chrome";
    chrome.ka = { glm::vec3(0.25f) };
    chrome.kd = { glm::vec3(0.4f) };
    chrome.ks = { glm::vec3(0.774597f) };
    chrome.ns = 0.6f;
    m_materials.emplace(chrome.newmtl, chrome);
}

void sg::ogl::resource::ModelManager::AddStaticMeshes()
{
    AddSkyboxStaticMesh();
    AddGuiStaticMesh();
    AddWaterStaticMesh();
    AddQuadStaticMesh();
    AddSunQuadStaticMesh();
    AddTerrainPatchStaticMesh();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::resource::ModelManager::AddSkyboxStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddSkyboxStaticMesh()] Add Skybox mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
    };

    // create vertices
    auto vertices{ CreateSkyboxVertices(500.0f) };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(reinterpret_cast<float*>(vertices.data()), static_cast<int32_t>(vertices.size()), bufferLayout);

    // store Mesh
    m_staticMeshes.emplace(SKYBOX_MESH, meshSharedPtr);
}

void sg::ogl::resource::ModelManager::AddGuiStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddGuiStaticMesh()] Add Gui mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // to render with GL_TRIANGLE_STRIP
    std::vector<float> vertices{
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f
    };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(vertices.data(), static_cast<int32_t>(vertices.size()) / 2, bufferLayout);

    // store Mesh
    m_staticMeshes.emplace(GUI_MESH, meshSharedPtr);
}

void sg::ogl::resource::ModelManager::AddWaterStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddWaterStaticMesh()] Add Water mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // x and z vertex positions, y is set to 0.0 in the vertex shader
    std::vector<float> vertices{
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(vertices.data(), static_cast<int32_t>(vertices.size()) / 2, bufferLayout);

    // store Mesh
    m_staticMeshes.emplace(WATER_MESH, meshSharedPtr);
}

void sg::ogl::resource::ModelManager::AddQuadStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddQuadStaticMesh()] Add Quad mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::UV, "aUv" },
    };

    // to render with GL_TRIANGLE_STRIP
    std::vector<float> vertices{
        // position         // uv
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // add Vbo
    const auto drawCount{ 4 };
    meshSharedPtr->GetVao().AddVertexDataVbo(vertices.data(), drawCount, bufferLayout);

    // store Mesh
    m_staticMeshes.emplace(QUAD_MESH, meshSharedPtr);
}

void sg::ogl::resource::ModelManager::AddSunQuadStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddSunQuadStaticMesh()] Add Sun quad mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // to render with GL_TRIANGLE_STRIP
    std::vector<float> vertices{
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f
    };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(vertices.data(), static_cast<int32_t>(vertices.size()) / 2, bufferLayout);

    // store Mesh
    m_staticMeshes.emplace(SUN_QUAD_MESH, meshSharedPtr);
}

void sg::ogl::resource::ModelManager::AddTerrainPatchStaticMesh()
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddTerrainPatchStaticMesh()] Add Terrain Patch mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
    };

    // set vertices
    std::vector<float> vertices{
        0.0f,   0.0f,
        0.333f, 0.0f,
        0.666f, 0.0f,
        1.0f,   0.0f,

        0.0f,   0.333f,
        0.333f, 0.333f,
        0.666f, 0.333f,
        1.0f,   0.333f,

        0.0f,   0.666f,
        0.333f, 0.666f,
        0.666f, 0.666f,
        1.0f,   0.666f,

        0.0f,   1.0f,
        0.333f, 1.0f,
        0.666f, 1.0f,
        1.0f,   1.0f,
    };

    // add Vbo
    const auto drawCount{ 16 };
    meshSharedPtr->GetVao().AddVertexDataVbo(vertices.data(), drawCount, bufferLayout);

    meshSharedPtr->GetVao().BindVao();
    glPatchParameteri(GL_PATCH_VERTICES, drawCount);
    meshSharedPtr->GetVao().UnbindVao();

    // store Mesh
    m_staticMeshes.emplace(TERRAIN_PATCH_MESH, meshSharedPtr);
}

std::vector<glm::vec3> sg::ogl::resource::ModelManager::CreateSkyboxVertices(const float t_size)
{
    return std::vector<glm::vec3>
    {
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, t_size)
    };
}
