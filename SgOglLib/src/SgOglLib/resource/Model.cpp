#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "Log.h"
#include "TextureManager.h"
#include "SgOglException.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::resource::DeleteMesh::operator()(Mesh* t_mesh) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteMesh::operator()] Delete Mesh.");
    delete t_mesh;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_fullFilePath, TextureManager& t_textureManager, const bool t_useTextures)
    : m_fullFilePath{ std::move(t_fullFilePath) }
    , m_textureManager{ t_textureManager }
    , m_useTextures{ t_useTextures }
{
    SG_OGL_CORE_LOG_DEBUG("[Model::Model()] Create Model.");

    m_directory = m_fullFilePath.substr(0, m_fullFilePath.find_last_of('/'));

    LoadModel();
}

sg::ogl::resource::Model::~Model() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Model::~Model()] Destruct Model.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::resource::Model::Meshes& sg::ogl::resource::Model::GetMeshes() const
{
    return m_meshes;
}

sg::ogl::resource::Model::Meshes& sg::ogl::resource::Model::GetMeshes()
{
    return m_meshes;
}

//-------------------------------------------------
// Load Model
//-------------------------------------------------

void sg::ogl::resource::Model::LoadModel()
{
    Assimp::Importer importer;

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Start loading the model: {}", m_fullFilePath);

    const auto* scene{ importer.ReadFile(m_fullFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals) };
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw SG_OGL_EXCEPTION("[Model::LoadModel()] ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }

    ProcessNode(scene->mRootNode, scene);

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Model {} successfully loaded.", m_fullFilePath);
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
    VerticesContainer vertices;
    IndicesContainer indices;

    // Create a Material instance.
    auto materialUniquePtr{ std::make_unique<Material>() };
    SG_OGL_CORE_ASSERT(materialUniquePtr, "[Model::ProcessMesh()] Null pointer.")

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
            SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing texture coords.");
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
            SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing tangent coords.");
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
            SG_OGL_LOG_WARN("[Model::ProcessMesh()] Missing bitangent coords.");
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

    // Set material name.
    aiString name;
    auto result = aiMeshMaterial->Get(AI_MATKEY_NAME, name);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[Model::ProcessMesh()] Unable to get the material name.");
    }
    materialUniquePtr->newmtl = name.C_Str();

    // Set ambient color.
    aiColor3D ambient;
    result = aiMeshMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[Model::ProcessMesh()] Unable to get the ambient color.");
    }
    materialUniquePtr->ka.x = ambient.r;
    materialUniquePtr->ka.y = ambient.g;
    materialUniquePtr->ka.z = ambient.b;

    // Set diffuse color.
    aiColor3D diffuse;
    result = aiMeshMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[Model::ProcessMesh()] Unable to get the diffuse color.");
    }
    materialUniquePtr->kd.x = diffuse.r;
    materialUniquePtr->kd.y = diffuse.g;
    materialUniquePtr->kd.z = diffuse.b;

    // Set specular color.
    aiColor3D specular;
    result = aiMeshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[Model::ProcessMesh()] Unable to get the specular color.");
    }
    materialUniquePtr->ks.x = specular.r;
    materialUniquePtr->ks.y = specular.g;
    materialUniquePtr->ks.z = specular.b;

    // Set the shininess.
    float shininess;
    result = aiMeshMaterial->Get(AI_MATKEY_SHININESS, shininess);
    if (result == aiReturn_FAILURE)
    {
        throw SG_OGL_EXCEPTION("[Model::ProcessMesh()] Unable to get the shininess.");
    }
    materialUniquePtr->ns = shininess;

    // Load material textures.
    if (m_useTextures)
    {
        SG_OGL_LOG_DEBUG("[Model::ProcessMesh()] Loading textures for the model: {}", m_fullFilePath);

        auto ambientMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_AMBIENT) };
        auto diffuseMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_DIFFUSE) };
        auto specularMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_SPECULAR) };
        auto normalMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_HEIGHT) }; // HeightMap = BumpMap/NormalMap

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

        if (!normalMaps.empty())
        {
            materialUniquePtr->mapBump = normalMaps[0];
        }
    }

    // todo
    // Use a predefined BufferLayout.
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" },
        { buffer::VertexAttributeType::TANGENT, "aTangent" },
        { buffer::VertexAttributeType::BITANGENT, "aBiTangent" },
    };

    // Create a Mesh instance.
    MeshUniquePtr meshUniquePtr;
    meshUniquePtr.reset(new Mesh);
    SG_OGL_CORE_ASSERT(meshUniquePtr, "[Model::ProcessMesh()] Null pointer.")

    // Allocate the data to the mesh.
    meshUniquePtr->Allocate(bufferLayout, &vertices, t_mesh->mNumVertices, &indices);

    // Set the material properties.
    meshUniquePtr->SetMaterial(std::move(materialUniquePtr));

    // Return a mesh object created from the extracted mesh data.
    return meshUniquePtr;
}

sg::ogl::resource::Model::TexturesContainer sg::ogl::resource::Model::LoadMaterialTextures(aiMaterial* t_mat, const aiTextureType t_type) const
{
    TexturesContainer textures;

    for (auto i{ 0u }; i < t_mat->GetTextureCount(t_type); ++i)
    {
        aiString str;
        const auto result{ t_mat->GetTexture(t_type, i, &str) };
        if (result == aiReturn_FAILURE)
        {
            throw SG_OGL_EXCEPTION("[Model::LoadMaterialTextures()] Error while loading material texture.");
        }

        textures.push_back(m_textureManager.GetTextureIdFromPath(m_directory + "/" + str.C_Str()));
    }

    return textures;
}
