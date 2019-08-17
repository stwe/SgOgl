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
        using ModelUniquePtr = std::unique_ptr<Model>;
        using ModelFullPathKey = std::string;
        using ModelContainer = std::map<ModelFullPathKey, ModelUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ModelManager() = delete;

        explicit ModelManager(TextureManager& t_textureManager);

        ModelManager(const ModelManager& other) = delete;
        ModelManager(ModelManager&& other) noexcept = delete;
        ModelManager& operator=(const ModelManager& other) = delete;
        ModelManager& operator=(ModelManager&& other) noexcept = delete;

        ~ModelManager() noexcept;

        //-------------------------------------------------
        // Load && Create
        //-------------------------------------------------

        Model* GetModelFromPath(const std::string& t_path);

    protected:

    private:
        TextureManager& m_textureManager;
        ModelContainer m_models;
    };
}
