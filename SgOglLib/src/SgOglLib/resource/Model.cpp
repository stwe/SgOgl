// This file is part of the SgOgl package.
// 
// Filename: Model.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <assimp/Importer.hpp>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Log.h"
#include "TextureManager.h"
#include "SgOglException.h"
#include "Application.h"
#include "Core.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_fullFilePath, Application* t_application, const unsigned int t_pFlags)
    : m_application{ t_application }
    , m_fullFilePath{ std::move(t_fullFilePath) }
{
    SG_OGL_CORE_ASSERT(m_application, "[Model::Model()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[Model::Model()] Create Model.");

    m_directory = m_fullFilePath.substr(0, m_fullFilePath.find_last_of('/'));

    LoadModel(t_pFlags);
}

sg::ogl::resource::Model::~Model() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Model::~Model()] Destruct Model.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::resource::Model::MeshContainer& sg::ogl::resource::Model::GetMeshes() const noexcept
{
    return m_meshes;
}

//-------------------------------------------------
// Load Model
//-------------------------------------------------

void sg::ogl::resource::Model::LoadModel(const unsigned int t_pFlags)
{
    Assimp::Importer importer;

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Start loading model file at: {}", m_fullFilePath);

    const auto* scene{ importer.ReadFile(m_fullFilePath, t_pFlags) };
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw SG_OGL_EXCEPTION("[Model::LoadModel()] ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }

    ProcessNode(scene->mRootNode, scene);

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Model file at {} successfully loaded.", m_fullFilePath);
}

void sg::ogl::resource::Model::ProcessNode(aiNode* t_node, const aiScene* t_scene)
{
    // Process each mesh located at the current node.
    for (auto i{ 0u }; i < t_node->mNumMeshes; ++i)
    {
        auto* mesh{ t_scene->mMeshes[t_node->mMeshes[i]] };
        m_meshes.push_back(ProcessMesh(mesh, t_scene));
    }

    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
    for (auto i{ 0u }; i < t_node->mNumChildren; ++i)
    {
        ProcessNode(t_node->mChildren[i], t_scene);
    }
}

sg::ogl::resource::Model::MeshUniquePtr sg::ogl::resource::Model::ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const
{
    // Data to fill.
    VertexContainer vertices;
    IndexContainer indices;

    // Prevent duplicate warnings.
    auto missingUv{ false };
    auto missingTangent{ false };
    auto missingBiTangent{ false };

    // Walk through each of the mesh's vertices.
    for (auto i{ 0u }; i < t_mesh->mNumVertices; ++i)
    {
        // push position (3 floats)
        vertices.push_back(t_mesh->mVertices[i].x);
        vertices.push_back(t_mesh->mVertices[i].y);
        vertices.push_back(t_mesh->mVertices[i].z);

        // push normal (3 floats)
        vertices.push_back(t_mesh->mNormals[i].x);
        vertices.push_back(t_mesh->mNormals[i].y);
        vertices.push_back(t_mesh->mNormals[i].z);

        // texture coordinates (2 floats)
        if (t_mesh->mTextureCoords[0])
        {
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertices.push_back(t_mesh->mTextureCoords[0][i].x);
            vertices.push_back(t_mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingUv)
            {
                missingUv = true;
                SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing texture coords. Set default values (0, 0).");
            }
        }

        // tangent (3 floats)
        if (t_mesh->mTangents)
        {
            vertices.push_back(t_mesh->mTangents[i].x);
            vertices.push_back(t_mesh->mTangents[i].y);
            vertices.push_back(t_mesh->mTangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingTangent)
            {
                missingTangent = true;
                SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing tangent coords. Set default values (0, 0, 0).");
            }
        }

        // bitangent (3 floats)
        if (t_mesh->mBitangents)
        {
            vertices.push_back(t_mesh->mBitangents[i].x);
            vertices.push_back(t_mesh->mBitangents[i].y);
            vertices.push_back(t_mesh->mBitangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingBiTangent)
            {
                missingBiTangent = true;
                SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing bitangent coords. Set default values (0, 0, 0).");
            }
        }
    }

    // Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (auto i{ 0u }; i < t_mesh->mNumFaces; ++i)
    {
        const auto face{ t_mesh->mFaces[i] };

        // Retrieve all indices of the face and store them in the indices vector.
        for (auto j{ 0u }; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process materials.
    auto* aiMeshMaterial{ t_scene->mMaterials[t_mesh->mMaterialIndex] };

    // Create a unique_ptr Material instance.
    auto materialUniquePtr{ std::make_unique<Material>() };
    SG_OGL_CORE_ASSERT(materialUniquePtr, "[Model::ProcessMesh()] Null pointer.")

    // Set material name.
    aiString name;
    aiMeshMaterial->Get(AI_MATKEY_NAME, name);
    materialUniquePtr->newmtl = name.C_Str();

    // Set material colors.
    aiColor3D color;
    aiMeshMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
    materialUniquePtr->ka = glm::vec3(color.r, color.g, color.b);
    aiMeshMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    materialUniquePtr->kd = glm::vec3(color.r, color.g, color.b);
    aiMeshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
    materialUniquePtr->ks = glm::vec3(color.r, color.g, color.b);

    // Set the material shininess.
    float shininess;
    aiMeshMaterial->Get(AI_MATKEY_SHININESS, shininess);
    materialUniquePtr->ns = shininess;

    // Load textures.
    SG_OGL_LOG_DEBUG("[Model::ProcessMesh()] Loading textures for the model: {}", m_fullFilePath);

    auto ambientMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_AMBIENT) };
    auto diffuseMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_DIFFUSE) };
    auto specularMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_SPECULAR) };
    auto bumpMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_HEIGHT) };
    auto normalMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_NORMALS) };

    // Always use the first texture Id.
    if (!ambientMaps.empty())
    {
        materialUniquePtr->mapKa = ambientMaps[0];
    }

    if (!diffuseMaps.empty())
    {
        materialUniquePtr->mapKd = diffuseMaps[0];
    }

    if (!specularMaps.empty())
    {
        materialUniquePtr->mapKs = specularMaps[0];
    }

    if (!bumpMaps.empty())
    {
        materialUniquePtr->mapBump = bumpMaps[0];
    }

    if (!normalMaps.empty())
    {
        materialUniquePtr->mapKn = normalMaps[0];
    }

    // Set the BufferLayout.
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" },
        { buffer::VertexAttributeType::TANGENT, "aTangent" },
        { buffer::VertexAttributeType::BITANGENT, "aBiTangent" },
    };

    // Create a unique_ptr Mesh instance.
    auto meshUniquePtr{ std::make_unique<Mesh>() };
    SG_OGL_CORE_ASSERT(meshUniquePtr, "[Model::ProcessMesh()] Null pointer.")

    // Add Vbo.
    meshUniquePtr->GetVao().AddVertexDataVbo(vertices.data(), t_mesh->mNumVertices, bufferLayout);

    // Add Ebo.
    meshUniquePtr->GetVao().AddIndexBuffer(indices);

    // Each mesh has a default material. Set the material properties as default.
    meshUniquePtr->SetDefaultMaterial(std::move(materialUniquePtr));

    // Return a mesh object created from the extracted mesh data.
    return meshUniquePtr;
}

sg::ogl::resource::Model::TextureContainer sg::ogl::resource::Model::LoadMaterialTextures(aiMaterial* t_mat, const aiTextureType t_type) const
{
    TextureContainer textures;

    for (auto i{ 0u }; i < t_mat->GetTextureCount(t_type); ++i)
    {
        aiString str;
        const auto result{ t_mat->GetTexture(t_type, i, &str) };
        if (result == aiReturn_FAILURE)
        {
            throw SG_OGL_EXCEPTION("[Model::LoadMaterialTextures()] Error while loading material texture.");
        }

        textures.push_back(m_application->GetTextureManager().GetTextureIdFromPath(m_directory + "/" + str.C_Str()));
    }

    return textures;
}
