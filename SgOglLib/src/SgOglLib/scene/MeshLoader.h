#pragma once

#include <assimp/scene.h>
#include <vector>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class Mesh;
    struct Material;
    class TextureManager;
}

namespace sg::ogl::scene
{
    class Node;

    class SG_OGL_API MeshLoader
    {
    public:
        using VerticesContainer = std::vector<float>;
        using IndicesContainer = std::vector<uint32_t>;
        using MeshContainer = std::vector<resource::Mesh*>;
        using TexturesContainer = std::vector<uint32_t>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MeshLoader() = delete;

        explicit MeshLoader(resource::TextureManager& t_textureManager);

        MeshLoader(const MeshLoader& t_other) = delete;
        MeshLoader(MeshLoader&& t_other) noexcept = delete;
        MeshLoader& operator=(const MeshLoader& t_other) = delete;
        MeshLoader& operator=(MeshLoader&& t_other) noexcept = delete;

        ~MeshLoader() noexcept;

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        Node* LoadMesh(const std::string& t_fullFilePath, bool t_setDefaultMaterial = true);

    protected:

    private:
        resource::TextureManager& m_textureManager;
        MeshContainer m_meshes;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        Node* ProcessNode(aiNode* t_aNode, const aiScene* t_aScene, const std::string& t_directory, bool t_setDefaultMaterial);
        resource::Mesh* ParseMesh(aiMesh* t_aMesh, const aiScene* t_aScene);
        resource::Material* ParseMaterial(aiMaterial* t_aMaterial, const aiScene* t_aScene, const std::string& t_directory) const;
        TexturesContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type, const std::string& t_directory) const;
    };
}
