#pragma once

#include <memory>
#include <map>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class TextureManager;
    class Model;

    class SG_OGL_API ModelManager
    {
    public:
        using ModelSharedPtr = std::shared_ptr<Model>;
        using ModelFullPathAsKey = std::string;
        using ModelContainer = std::map<ModelFullPathAsKey, ModelSharedPtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ModelManager() = delete;

        explicit ModelManager(TextureManager& t_textureManager);

        ModelManager(const ModelManager& t_other) = delete;
        ModelManager(ModelManager&& t_other) noexcept = delete;
        ModelManager& operator=(const ModelManager& t_other) = delete;
        ModelManager& operator=(ModelManager&& t_other) noexcept = delete;

        ~ModelManager() noexcept;

        //-------------------------------------------------
        // Load && Create
        //-------------------------------------------------

        ModelSharedPtr GetModelFromPath(const std::string& t_fullFilePath);

    protected:

    private:
        TextureManager& m_textureManager;
        ModelContainer m_models;
    };
}
