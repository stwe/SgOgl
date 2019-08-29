#pragma once

#include <assimp/scene.h>
#include <vector>
#include <memory>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class TextureManager;
    class Mesh;

    class SG_OGL_API Model
    {
    public:
        // private Model Ctor. -> Use the ModelManager to create a new Model
        friend class ModelManager;

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
        MeshContainer m_meshes;

        std::string m_fullFilePath;
        std::string m_directory;

        TextureManager& m_textureManager;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model(std::string t_fullFilePath, TextureManager& t_textureManager);

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        void LoadModel();
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const;
        TextureContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;
    };
}
