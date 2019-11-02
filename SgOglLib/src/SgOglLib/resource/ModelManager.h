// This file is part of the SgOgl package.
// 
// Filename: ModelManager.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <map>
#include <string>
#include <vector>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::resource
{
    struct Material;
    class Mesh;
    class Model;

    class ModelManager
    {
    public:
        using MaterialSharedPtr = std::shared_ptr<Material>;
        using MaterialKey = std::string;
        using MaterialContainer = std::map<MaterialKey, MaterialSharedPtr>;

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

        MaterialSharedPtr GetMaterialByName(const std::string& t_name);
        StaticMeshSharedPtr GetStaticMeshByName(const std::string& t_name);
        ModelSharedPtr GetModelByPath(const std::string& t_fullFilePath);

        //-------------------------------------------------
        // Add
        //-------------------------------------------------

        void AddMaterial(const MaterialSharedPtr& t_material);
        void AddStaticMesh(const StaticMeshSharedPtr& t_staticMesh);

    protected:

    private:
        Application* m_application{ nullptr };

        MaterialContainer m_materials;
        StaticMeshContainer m_staticMeshes;
        ModelContainer m_models;

        //-------------------------------------------------
        // Built in
        //-------------------------------------------------

        void AddMaterials();
        void AddStaticMeshes();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void AddSkyboxStaticMesh();

        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size = 500.0f);
    };
}
