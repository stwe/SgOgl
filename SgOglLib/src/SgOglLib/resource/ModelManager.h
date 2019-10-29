#pragma once

#include <memory>
#include <map>
#include <string>
#include "Core.h"

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::resource
{
    class Mesh;
    class Model;

    class SG_OGL_API ModelManager
    {
    public:
        using StaticMeshSharedPtr = std::shared_ptr<Mesh>;
        using StaticMeshKey = std::string;
        using StaticMeshContainer = std::map<StaticMeshKey, StaticMeshSharedPtr>;

        using ModelSharedPtr = std::shared_ptr<Model>;
        using ModelFullPathAsKey = std::string;
        using ModelContainer = std::map<ModelFullPathAsKey, ModelSharedPtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ModelManager() = delete;

        explicit ModelManager(Application* t_application);

        ModelManager(const ModelManager& t_other) = delete;
        ModelManager(ModelManager&& t_other) noexcept = delete;
        ModelManager& operator=(const ModelManager& t_other) = delete;
        ModelManager& operator=(ModelManager&& t_other) noexcept = delete;

        ~ModelManager() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        StaticMeshSharedPtr GetStaticMeshByName(const std::string& t_name);
        ModelSharedPtr GetModelByPath(const std::string& t_fullFilePath);

        //-------------------------------------------------
        // Add
        //-------------------------------------------------

        void AddStaticMesh(const StaticMeshSharedPtr& t_staticMesh);

    protected:

    private:
        Application* m_application{ nullptr };

        StaticMeshContainer m_staticMeshes;
        ModelContainer m_models;
    };
}
