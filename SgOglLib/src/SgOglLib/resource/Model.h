#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include <memory>
#include <string>

namespace sg::ogl::resource
{
    class Mesh;

    class Model
    {
    public:
        using MeshUniquePtr = std::unique_ptr<Mesh>;
        using Meshes = std::vector<MeshUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        explicit Model(std::string t_filePath, bool t_useTextures = true);

        Model(const Model& t_other) = delete;
        Model(Model&& t_other) noexcept = delete;
        Model& operator=(const Model& t_other) = delete;
        Model& operator=(Model&& t_other) noexcept = delete;

        ~Model() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const Meshes& GetMeshes() const;
        Meshes& GetMeshes();

    protected:

    private:
        Meshes m_meshes;

        std::string m_fileName;
        std::string m_directory;

        bool m_useTextures{ true };

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        void LoadModel();
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const;
        //std::vector<uint32_t> LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;
    };
}
