#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshLoader.h"
#include "Log.h"
#include "SgOglException.h"
#include "scene/Node.h"
#include "buffer/BufferLayout.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "resource/Material.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::MeshLoader::MeshLoader(resource::TextureManager& t_textureManager)
	: m_textureManager{ t_textureManager }
{
}

sg::ogl::scene::MeshLoader::~MeshLoader() noexcept
{
}

//-------------------------------------------------
// Load
//-------------------------------------------------

sg::ogl::scene::Node* sg::ogl::scene::MeshLoader::LoadMesh(const std::string& t_fullFilePath, const bool t_setDefaultMaterial)
{
    Assimp::Importer importer;

    SG_OGL_LOG_DEBUG("[MeshLoader::LoadMesh()] Start loading model file at: {}", t_fullFilePath);

    const auto* scene{ importer.ReadFile(t_fullFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals) };
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::LoadMesh()] ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }

    SG_OGL_LOG_DEBUG("[MeshLoader::LoadMesh()] Model file at {} successfully loaded.", t_fullFilePath);

    const auto directory{ t_fullFilePath.substr(0, t_fullFilePath.find_last_of('/')) };

    return ProcessNode(scene->mRootNode, scene, directory, t_setDefaultMaterial);
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

sg::ogl::scene::Node* sg::ogl::scene::MeshLoader::ProcessNode(aiNode* t_aNode, const aiScene* t_aScene, const std::string& t_directory, const bool t_setDefaultMaterial)
{
	auto* node{ new Node };

    // Process each mesh located at the current node.
    for (auto i{ 0u }; i < t_aNode->mNumMeshes; ++i)
    {
        aiMesh* assimpMesh{ t_aScene->mMeshes[t_aNode->mMeshes[i]] };
        aiMaterial* assimpMat{ t_aScene->mMaterials[assimpMesh->mMaterialIndex] };

        resource::Mesh* mesh{ ParseMesh(assimpMesh, t_aScene) };
        resource::Material* material{ nullptr };
        if (t_setDefaultMaterial)
        {
            material = ParseMaterial(assimpMat, t_aScene, t_directory);
        }

        // If we only have one mesh, this node itself contains the mesh/material.
        if (t_aNode->mNumMeshes == 1)
        {
            node->mesh = mesh;
            if (t_setDefaultMaterial)
            {
                node->material = material;
            }
        }

        // Otherwise, the meshes are considered on equal depth of its children.
        else
        {
            auto* child{ new Node };
            child->mesh = mesh;
            child->material = material;
            node->AddChild(child);
        }
    }

    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
    for (auto i{ 0u }; i < t_aNode->mNumChildren; ++i)
    {
		auto* c{ ProcessNode(t_aNode->mChildren[i], t_aScene, t_directory, t_setDefaultMaterial) };
        node->AddChild(c);
    }

    return node;
}

sg::ogl::resource::Mesh* sg::ogl::scene::MeshLoader::ParseMesh(aiMesh* t_aMesh, const aiScene* t_aScene)
{
    // Data to fill.
    VerticesContainer vertices;
    IndicesContainer indices;

    // Walk through each of the mesh's vertices.
    for (auto i{ 0u }; i < t_aMesh->mNumVertices; ++i)
    {
        // push position (3 floats)
        vertices.push_back(t_aMesh->mVertices[i].x);
        vertices.push_back(t_aMesh->mVertices[i].y);
        vertices.push_back(t_aMesh->mVertices[i].z);

        // push normal (3 floats)
        vertices.push_back(t_aMesh->mNormals[i].x);
        vertices.push_back(t_aMesh->mNormals[i].y);
        vertices.push_back(t_aMesh->mNormals[i].z);

        // texture coordinates (2 floats)
        if (t_aMesh->mTextureCoords[0])
        {
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertices.push_back(t_aMesh->mTextureCoords[0][i].x);
            vertices.push_back(t_aMesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            SG_OGL_LOG_WARN("[MeshLoader::ParseMesh()] Missing texture coords.");
        }

        // tangent (3 floats)
        if (t_aMesh->mTangents)
        {
            vertices.push_back(t_aMesh->mTangents[i].x);
            vertices.push_back(t_aMesh->mTangents[i].y);
            vertices.push_back(t_aMesh->mTangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            SG_OGL_LOG_WARN("[MeshLoader::ParseMesh()] Missing tangent coords.");
        }

        // bitangent (3 floats)
        if (t_aMesh->mBitangents)
        {
            vertices.push_back(t_aMesh->mBitangents[i].x);
            vertices.push_back(t_aMesh->mBitangents[i].y);
            vertices.push_back(t_aMesh->mBitangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            SG_OGL_LOG_WARN("[MeshLoader::ParseMesh()] Missing bitangent coords.");
        }
    }

    // Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (auto f{ 0u }; f < t_aMesh->mNumFaces; ++f)
    {
        const auto face{ t_aMesh->mFaces[f] };

        // Retrieve all indices of the face and store them in the indices vector.
        for (auto j{ 0u }; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Use a predefined BufferLayout.
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" },
        { buffer::VertexAttributeType::TANGENT, "aTangent" },
        { buffer::VertexAttributeType::BITANGENT, "aBiTangent" },
    };

    // Create a Mesh instance.
    auto* mesh = new resource::Mesh;

    // Allocate the data to the mesh.
    mesh->Allocate(bufferLayout, &vertices, t_aMesh->mNumVertices, &indices);

    m_meshes.push_back(mesh);

    return mesh;
}

sg::ogl::resource::Material* sg::ogl::scene::MeshLoader::ParseMaterial(aiMaterial* t_aMaterial, const aiScene* t_aScene, const std::string& t_directory) const
{
    auto* material{ new sg::ogl::resource::Material };

    // Set material name.
    aiString name;
    auto result = t_aMaterial->Get(AI_MATKEY_NAME, name);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::ParseMaterial()] Unable to get the material name.");
    }
    material->newmtl = name.C_Str();

    // Set ambient color.
    aiColor3D ambient;
    result = t_aMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::ParseMaterial()] Unable to get the ambient color.");
    }
    material->ka.x = ambient.r;
    material->ka.y = ambient.g;
    material->ka.z = ambient.b;

    // Set diffuse color.
    aiColor3D diffuse;
    result = t_aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::ParseMaterial()] Unable to get the diffuse color.");
    }
    material->kd.x = diffuse.r;
    material->kd.y = diffuse.g;
    material->kd.z = diffuse.b;

    // Set specular color.
    aiColor3D specular;
    result = t_aMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::ParseMaterial()] Unable to get the specular color.");
    }
    material->ks.x = specular.r;
    material->ks.y = specular.g;
    material->ks.z = specular.b;

    // Set the shininess.
    float shininess;
    result = t_aMaterial->Get(AI_MATKEY_SHININESS, shininess);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[MeshLoader::ParseMaterial()] Unable to get the shininess.");
    }
    material->ns = shininess;

    // Load material textures.
    SG_OGL_LOG_DEBUG("[MeshLoader::ParseMaterial()] Loading textures for the model: {}", t_directory);

    auto ambientMaps{ LoadMaterialTextures(t_aMaterial, aiTextureType_AMBIENT, t_directory) };
    auto diffuseMaps{ LoadMaterialTextures(t_aMaterial, aiTextureType_DIFFUSE, t_directory) };
    auto specularMaps{ LoadMaterialTextures(t_aMaterial, aiTextureType_SPECULAR, t_directory) };
    auto normalMaps{ LoadMaterialTextures(t_aMaterial, aiTextureType_HEIGHT, t_directory) }; // HeightMap = BumpMap/NormalMap

    // Always use the first texture Id.
    if (!ambientMaps.empty())
    {
        material->mapKa = ambientMaps[0];
    }

    if (!diffuseMaps.empty())
    {
        material->mapKd = diffuseMaps[0];
    }

    if (!specularMaps.empty())
    {
        material->mapKs = specularMaps[0];
    }

    if (!normalMaps.empty())
    {
        material->mapBump = normalMaps[0];
    }

    return material;
}

sg::ogl::scene::MeshLoader::TexturesContainer sg::ogl::scene::MeshLoader::LoadMaterialTextures(aiMaterial* t_mat, const aiTextureType t_type, const std::string& t_directory) const
{
    TexturesContainer textures;

    for (auto i{ 0u }; i < t_mat->GetTextureCount(t_type); ++i)
    {
        aiString str;
        const auto result{ t_mat->GetTexture(t_type, i, &str) };
        if (result == aiReturn_FAILURE)
        {
            throw SG_OGL_EXCEPTION("[MeshLoader::LoadMaterialTextures()] Error while loading material texture.");
        }

        textures.push_back(m_textureManager.GetTextureIdFromPath(t_directory + "/" + str.C_Str()));
    }

    return textures;
}
