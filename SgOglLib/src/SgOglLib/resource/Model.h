#pragma once

#include <vector>
#include <memory>
#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace sg::ogl::resource
{
    class TextureManager;
    class Mesh;

    class SG_OGL_API Model
    {
    public:
        using VerticesContainer = std::vector<float>;
        using IndicesContainer = std::vector<uint32_t>;
        using TexturesContainer = std::vector<uint32_t>;
        using MeshUniquePtr = std::unique_ptr<Mesh>;
        using Meshes = std::vector<MeshUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        Model(std::string t_fullFilePath, TextureManager& t_textureManager, bool t_useTextures = true);

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

        std::string m_fullFilePath;
        std::string m_directory;

        TextureManager& m_textureManager;

        bool m_useTextures{ true };

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        void LoadModel();
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const;
        TexturesContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;
    };
}
