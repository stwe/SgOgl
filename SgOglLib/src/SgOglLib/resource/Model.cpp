#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Model.h"
#include "Log.h"
#include "SgOglException.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_filePath, const bool t_useTextures)
    : m_fileName{ std::move(t_filePath) }
    , m_useTextures{ t_useTextures }
{
    m_directory = m_fileName.substr(0, m_fileName.find_last_of('/'));

    LoadModel();
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

void sg::ogl::resource::Model::LoadModel()
{
    Assimp::Importer importer;

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Start loading the model: {}", m_fileName);

    const auto* scene{ importer.ReadFile(m_fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals) };
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw SG_OGL_EXCEPTION("[Model::LoadModel()] ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }

    ProcessNode(scene->mRootNode, scene);

    SG_OGL_LOG_DEBUG("[Model::LoadModel()] Model {} successfully loaded.", m_fileName);
}

void sg::ogl::resource::Model::ProcessNode(aiNode* t_node, const aiScene* t_scene)
{
    // Process each mesh located at the current node.
    for (auto i{ 0u }; i < t_node->mNumMeshes; ++i)
    {
        const auto mesh{ t_scene->mMeshes[t_node->mMeshes[i]] };
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
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

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

    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" },
        { buffer::VertexAttributeType::TANGENT, "aTangent" },
        { buffer::VertexAttributeType::BITANGENT, "aBiTangent" },
    };

    auto meshUniquePtr{ std::make_unique<Mesh>() };
    meshUniquePtr->Allocate(bufferLayout, &vertices, t_mesh->mNumVertices, &indices);

    // Return a mesh object created from the extracted mesh data.
    return meshUniquePtr;
}
