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

sg::ogl::resource::Model* sg::ogl::resource::ModelManager::GetModelFromPath(const std::string& t_path)
{
    if (m_models.count(t_path) == 0)
    {
        auto modelUniquePtr{ std::unique_ptr<Model>(new Model(t_path, m_textureManager)) };
        SG_OGL_CORE_ASSERT(modelUniquePtr, "[ModelManager::GetModelFromPath()] Null pointer.")

        m_models.emplace(t_path, std::move(modelUniquePtr));
    }

    return m_models.at(t_path).get();
}
