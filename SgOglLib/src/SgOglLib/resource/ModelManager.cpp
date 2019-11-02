// This file is part of the SgOgl package.
// 
// Filename: ModelManager.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "ModelManager.h"
#include "Material.h"
#include "Model.h"
#include "Mesh.h"
#include "Log.h"
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
    SG_OGL_CORE_ASSERT(m_application, "[ModelManager::ModelManager()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::ModelManager()] Create ModelManager.");

    AddMaterials();
    AddStaticMeshes();
}

sg::ogl::resource::ModelManager::~ModelManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::~ModelManager()] Destruct ModelManager.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::resource::ModelManager::MaterialSharedPtr sg::ogl::resource::ModelManager::GetMaterialByName(const std::string& t_name)
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

sg::ogl::resource::ModelManager::ModelSharedPtr sg::ogl::resource::ModelManager::GetModelByPath(const std::string& t_fullFilePath)
{
    // load the model if it does not already exist
    if (m_models.count(t_fullFilePath) == 0)
    {
        auto modelSharedPtr{ std::make_shared<Model>(t_fullFilePath, m_application) };
        SG_OGL_CORE_ASSERT(modelSharedPtr, "[ModelManager::GetModelByPath()] Null pointer.")

        m_models.emplace(t_fullFilePath, modelSharedPtr);
    }

    return m_models.at(t_fullFilePath);
}

//-------------------------------------------------
// Add
//-------------------------------------------------

void sg::ogl::resource::ModelManager::AddMaterial(const MaterialSharedPtr& t_material)
{
    if (m_materials.count(t_material->newmtl) > 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::AddMaterial()] The Material already exists.");
    }

    m_materials.emplace(t_material->newmtl, t_material);
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
    // add some example materials

    SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add emerald material.");

    Material emerald;
    emerald.newmtl = "emerald";
    emerald.ka = { glm::vec3(0.0215f, 0.1745f, 0.0215f) };
    emerald.kd = { glm::vec3(0.07568f, 0.61424f, 0.07568f) };
    emerald.ks = { glm::vec3(0.633f, 0.727811f, 0.633f) };
    emerald.ns = 0.6f;
    m_materials.emplace(emerald.newmtl, std::make_shared<Material>(emerald));

    SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddMaterials()] Add chrome material.");

    Material chrome;
    chrome.newmtl = "chrome";
    chrome.ka = { glm::vec3(0.25f) };
    chrome.kd = { glm::vec3(0.4f) };
    chrome.ks = { glm::vec3(0.774597f) };
    chrome.ns = 0.6f;
    m_materials.emplace(chrome.newmtl, std::make_shared<Material>(chrome));
}

void sg::ogl::resource::ModelManager::AddStaticMeshes()
{
    AddSkyboxStaticMesh();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::resource::ModelManager::AddSkyboxStaticMesh()
{
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::AddSkyboxStaticMesh()] Add Skybox mesh.");

    // create Mesh
    auto meshSharedPtr{ std::make_shared<Mesh>() };

    // create BufferLayout
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "vPosition" },
    };

    // create vertices
    auto vertices{ CreateSkyboxVertices(500.0f) };

    // add Vbo
    meshSharedPtr->GetVao().AddVertexDataVbo(reinterpret_cast<float*>(vertices.data()), static_cast<int32_t>(vertices.size()), bufferLayout);

    // store Mesh
    m_staticMeshes.emplace("skybox", meshSharedPtr);
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
