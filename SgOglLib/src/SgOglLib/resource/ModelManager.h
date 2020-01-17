// This file is part of the SgOgl package.
// 
// Filename: ModelManager.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <assimp/postprocess.h>
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
    class SkeletalModel;

    class ModelManager
    {
    public:
        using MaterialSharedPtr = std::shared_ptr<Material>;
        using MaterialKey = std::string;
        using MaterialContainer = std::map<MaterialKey, MaterialSharedPtr>;

        using StaticMeshSharedPtr = std::shared_ptr<Mesh>;
        using StaticMeshKey = std::string;
        using StaticMeshContainer = std::map<StaticMeshKey, StaticMeshSharedPtr>;

        using FullPathAsKey = std::string;

        using ModelSharedPtr = std::shared_ptr<Model>;
        using ModelContainer = std::map<FullPathAsKey, ModelSharedPtr>;

        using SkeletalModelSharedPtr = std::shared_ptr<SkeletalModel>;
        using SkeletalModelContainer = std::map<FullPathAsKey, SkeletalModelSharedPtr>;

        using VertexContainer = std::vector<float>;

        //-------------------------------------------------
        // Built in mesh names
        //-------------------------------------------------

        inline static const StaticMeshKey SKYBOX_MESH{ "skybox" };
        inline static const StaticMeshKey GUI_MESH{ "gui" };
        inline static const StaticMeshKey WATER_MESH{ "water" };
        inline static const StaticMeshKey QUAD_MESH{ "quad" };
        inline static const StaticMeshKey SUN_QUAD_MESH{ "sun_quad" };
        inline static const StaticMeshKey TERRAIN_PATCH_MESH{ "terrain_patch" };

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
        ModelSharedPtr GetModelByPath(const std::string& t_fullFilePath, unsigned int t_pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
        SkeletalModelSharedPtr GetSkeletalModelByPath(const std::string& t_fullFilePath, unsigned int t_pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

        static VertexContainer GetParticleVertices();

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
        SkeletalModelContainer m_skeletalModels;

        //-------------------------------------------------
        // Built in
        //-------------------------------------------------

        void AddMaterials();
        void AddStaticMeshes();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void AddSkyboxStaticMesh();
        void AddGuiStaticMesh();
        void AddWaterStaticMesh();
        void AddQuadStaticMesh();
        void AddSunQuadStaticMesh();
        void AddTerrainPatchStaticMesh();

        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size = 500.0f);
    };
}
