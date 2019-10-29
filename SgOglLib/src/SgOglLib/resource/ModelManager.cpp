#include "ModelManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Log.h"
#include "Application.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ModelManager::ModelManager(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[ModelManager::ModelManager()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::ModelManager()] Create ModelManager.");
}

sg::ogl::resource::ModelManager::~ModelManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::~ModelManager()] Destruct ModelManager.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

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

void sg::ogl::resource::ModelManager::AddStaticMesh(const StaticMeshSharedPtr& t_staticMesh)
{
    if (m_staticMeshes.count(t_staticMesh->GetName()) > 0)
    {
        throw SG_OGL_EXCEPTION("[ModelManager::AddStaticMesh()] The Mesh already exists.");
    }

    m_staticMeshes.emplace(t_staticMesh->GetName(), t_staticMesh);
}
