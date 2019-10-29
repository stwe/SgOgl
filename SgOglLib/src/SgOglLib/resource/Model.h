// This file is part of the SgOgl package.
// 
// Filename: Model.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <assimp/scene.h>
#include <vector>
#include <memory>
#include <string>
#include "Core.h"

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::resource
{
    class Mesh;

    class SG_OGL_API Model
    {
    public:
        using VertexContainer = std::vector<float>;
        using IndexContainer = std::vector<uint32_t>;
        using TextureContainer = std::vector<uint32_t>;
        using MeshUniquePtr = std::unique_ptr<Mesh>;
        using MeshSharedPtr = std::shared_ptr<Mesh>;
        using MeshContainer = std::vector<MeshSharedPtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        Model(std::string t_fullFilePath, Application* t_application);

        Model(const Model& t_other) = delete;
        Model(Model&& t_other) noexcept = delete;
        Model& operator=(const Model& t_other) = delete;
        Model& operator=(Model&& t_other) noexcept = delete;

        ~Model() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const MeshContainer& GetMeshes() const noexcept;

    protected:

    private:
        Application* m_application{ nullptr };

        MeshContainer m_meshes;

        std::string m_fullFilePath;
        std::string m_directory;

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        void LoadModel();
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const;
        TextureContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;
    };
}
