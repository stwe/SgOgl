#include "ModelManager.h"
#include "Model.h"
#include "Log.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ModelManager::ModelManager(TextureManager& t_textureManager)
    : m_textureManager{ t_textureManager }
{
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::ModelManager()] Create ModelManager.");
}

sg::ogl::resource::ModelManager::~ModelManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ModelManager::~ModelManager()] Destruct ModelManager.");
}

//-------------------------------------------------
// Load && Create
//-------------------------------------------------

sg::ogl::resource::ModelManager::ModelSharedPtr sg::ogl::resource::ModelManager::GetModelFromPath(const std::string& t_fullFilePath)
{
    // create Model only if necessary
    if (m_models.count(t_fullFilePath) == 0)
    {
        auto modelSharedPtr{ std::shared_ptr<Model>(new Model(t_fullFilePath, m_textureManager)) };
        SG_OGL_CORE_ASSERT(modelSharedPtr, "[ModelManager::GetModelFromPath()] Null pointer.")

        m_models.emplace(t_fullFilePath, modelSharedPtr);
    }

    // return existing Model
    return m_models.at(t_fullFilePath);
}
