// This file is part of the SgOgl package.
// 
// Filename: SkeletalModel.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtx/quaternion.hpp>
#include "SkeletalModel.h"
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

sg::ogl::resource::SkeletalModel::SkeletalModel(std::string t_fullFilePath, Application* t_application, const unsigned int t_pFlags)
    : m_application{ t_application }
    , m_fullFilePath{ std::move(t_fullFilePath) }
{
    SG_OGL_CORE_ASSERT(m_application, "[SkeletalModel::SkeletalModel()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[SkeletalModel::SkeletalModel()] Create SkeletalModel.");

    m_directory = m_fullFilePath.substr(0, m_fullFilePath.find_last_of('/'));

    LoadModel(t_pFlags);
}

sg::ogl::resource::SkeletalModel::~SkeletalModel() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[SkeletalModel::~SkeletalModel()] Destruct SkeletalModel.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::resource::SkeletalModel::MeshContainer& sg::ogl::resource::SkeletalModel::GetMeshes() const noexcept
{
    return m_meshes;
}

uint32_t sg::ogl::resource::SkeletalModel::GetNumberOfAnimations() const
{
    SG_OGL_CORE_ASSERT(m_scene, "[SkeletalModel::GetNumberOfAnimations()] Null pointer.")
    return m_scene->mNumAnimations;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::resource::SkeletalModel::SetCurrentAnimation(const uint32_t t_animation)
{
    SG_OGL_CORE_ASSERT(t_animation <= m_scene->mNumAnimations, "[SkeletalModel::SetCurrentAnimation()] Invalid animation value.")
    m_currentAnimation = t_animation;
}

//-------------------------------------------------
// Transform
//-------------------------------------------------

void sg::ogl::resource::SkeletalModel::BoneTransform(const double t_timeInSec, std::vector<glm::mat4>& t_transforms)
{
    // calc animation duration
    const auto numPosKeys{ m_scene->mAnimations[m_currentAnimation]->mChannels[0]->mNumPositionKeys };
    const auto animDuration{ m_scene->mAnimations[m_currentAnimation]->mChannels[0]->mPositionKeys[numPosKeys - 1].mTime };

    const auto ticksPerSecond{ static_cast<float>(m_scene->mAnimations[m_currentAnimation]->mTicksPerSecond != 0 ? m_scene->mAnimations[m_currentAnimation]->mTicksPerSecond : 25.0f) };
    const auto timeInTicks{ t_timeInSec * ticksPerSecond };
    const auto animationTime{ static_cast<float>(fmod(timeInTicks, animDuration)) };

    ReadNodeHierarchy(animationTime, m_scene->mRootNode, glm::mat4(1.0f));

    t_transforms.resize(m_numBones);

    for (auto i{ 0u }; i < m_numBones; ++i)
    {
        t_transforms[i] = m_boneMatrices[i].finalWorldTransform;
    }
}

//-------------------------------------------------
// Load Model
//-------------------------------------------------

void sg::ogl::resource::SkeletalModel::LoadModel(const unsigned int t_pFlags)
{
    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Start loading skeletal model file at: {}", m_fullFilePath);

    m_scene = m_importer.ReadFile(m_fullFilePath, t_pFlags);
    if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
    {
        throw SG_OGL_EXCEPTION("[SkeletalModel::LoadModel()] ERROR::ASSIMP:: " + std::string(m_importer.GetErrorString()));
    }

    m_globalInverseTransform = mat4_cast(m_scene->mRootNode->mTransformation);
    m_globalInverseTransform = inverse(m_globalInverseTransform);

    if (!m_scene->mAnimations)
    {
        throw SG_OGL_EXCEPTION("[SkeletalModel::LoadModel()] The model should contain animations.");
    }

    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Num meshes: {}", m_scene->mNumMeshes);
    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Num animations: {}", m_scene->mNumAnimations);
    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Num channels: {}", m_scene->mAnimations[0]->mNumChannels);
    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Duration: {}", m_scene->mAnimations[0]->mDuration);

    ProcessNode(m_scene->mRootNode, m_scene);

    SG_OGL_LOG_DEBUG("[SkeletalModel::LoadModel()] Skeletal model file at {} successfully loaded.", m_fullFilePath);
}

void sg::ogl::resource::SkeletalModel::ProcessNode(aiNode* t_node, const aiScene* t_scene)
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

sg::ogl::resource::SkeletalModel::MeshUniquePtr sg::ogl::resource::SkeletalModel::ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene)
{
    SG_OGL_LOG_DEBUG("[SkeletalModel::ProcessMesh()] Mesh bones: {}", t_mesh->mNumBones);
    SG_OGL_LOG_DEBUG("[SkeletalModel::ProcessMesh()] Mesh vertices: {}", t_mesh->mNumVertices);

    // Data to fill.
    VertexContainer vertices;
    IndexContainer indices;
    VertexBonesContainer vertexBones;

    vertexBones.resize(t_mesh->mNumVertices);

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
                SG_OGL_LOG_WARN("[SkeletalModel::ProcessMesh()] Missing texture coords. Set default values (0, 0).");
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
                SG_OGL_LOG_WARN("[SkeletalModel::ProcessMesh()] Missing tangent coords. Set default values (0, 0, 0).");
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
                SG_OGL_LOG_WARN("[SkeletalModel::ProcessMesh()] Missing bitangent coords. Set default values (0, 0, 0).");
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

    // Walk through each of the mesh's bones.
    for (auto i{ 0u }; i < t_mesh->mNumBones; ++i)
    {
        auto boneIndex{ 0u };
        std::string boneName{ t_mesh->mBones[i]->mName.data };

        SG_OGL_LOG_DEBUG("[SkeletalModel::ProcessMesh()] Load Bone: {}", boneName);

        if (m_boneContainer.count(boneName) == 0)
        {
            boneIndex = m_numBones;
            m_numBones++;

            BoneMatrix boneMatrix;
            m_boneMatrices.push_back(boneMatrix);

            m_boneMatrices[boneIndex].offsetMatrix = mat4_cast(t_mesh->mBones[i]->mOffsetMatrix);
            m_boneContainer.emplace(boneName, boneIndex);
        }
        else
        {
            boneIndex = m_boneContainer.at(boneName);
        }

        for (auto j{ 0u }; j < t_mesh->mBones[i]->mNumWeights; ++j)
        {
            auto vertexId{ t_mesh->mBones[i]->mWeights[j].mVertexId };
            auto weight{ t_mesh->mBones[i]->mWeights[j].mWeight };
            vertexBones[vertexId].Add(boneIndex, weight);
        }
    }

    // Process materials.
    auto* aiMeshMaterial{ t_scene->mMaterials[t_mesh->mMaterialIndex] };

    // Create a unique_ptr Material instance.
    auto materialUniquePtr{ std::make_unique<Material>() };
    SG_OGL_CORE_ASSERT(materialUniquePtr, "[SkeletalModel::ProcessMesh()] Null pointer.")

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
    SG_OGL_LOG_DEBUG("[SkeletalModel::ProcessMesh()] Loading textures for the model: {}", m_fullFilePath);

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

    // Set the BufferLayouts.
    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "aPosition" },
        { buffer::VertexAttributeType::NORMAL, "aNormal" },
        { buffer::VertexAttributeType::UV, "aUv" },
        { buffer::VertexAttributeType::TANGENT, "aTangent" },
        { buffer::VertexAttributeType::BITANGENT, "aBiTangent" },
    };

    const buffer::BufferLayout bonesBufferLayout{
        { buffer::VertexAttributeType::BONE_IDS, "aBoneIds" },
        { buffer::VertexAttributeType::WEIGHTS, "aWeights" },
    };

    // Create a unique_ptr Mesh instance.
    auto meshUniquePtr{ std::make_unique<Mesh>() };
    SG_OGL_CORE_ASSERT(meshUniquePtr, "[SkeletalModel::ProcessMesh()] Null pointer.")

    // Add Vbos.
    meshUniquePtr->GetVao().AddVertexDataVbo(vertices.data(), t_mesh->mNumVertices, bufferLayout);
    meshUniquePtr->GetVao().AddDataVbo(vertexBones, bonesBufferLayout, 5);

    // Add Ebo.
    meshUniquePtr->GetVao().AddIndexBuffer(indices);

    // Each mesh has a default material. Set the material properties as default.
    meshUniquePtr->SetDefaultMaterial(std::move(materialUniquePtr));

    // Return a mesh object created from the extracted mesh data.
    return meshUniquePtr;
}

sg::ogl::resource::SkeletalModel::TextureContainer sg::ogl::resource::SkeletalModel::LoadMaterialTextures(aiMaterial* t_mat, const aiTextureType t_type) const
{
    TextureContainer textures;

    for (auto i{ 0u }; i < t_mat->GetTextureCount(t_type); ++i)
    {
        aiString str;
        const auto result{ t_mat->GetTexture(t_type, i, &str) };
        if (result == aiReturn_FAILURE)
        {
            throw SG_OGL_EXCEPTION("[SkeletalModel::LoadMaterialTextures()] Error while loading material texture.");
        }

        textures.push_back(m_application->GetTextureManager().GetTextureIdFromPath(m_directory + "/" + str.C_Str()));
    }

    return textures;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

const aiNodeAnim* sg::ogl::resource::SkeletalModel::FindNodeAnim(const aiAnimation* t_animation, const std::string& t_nodeName)
{
    for (auto i{ 0u }; i < t_animation->mNumChannels; ++i)
    {
        const aiNodeAnim* nodeAnim{ t_animation->mChannels[i] };
        if (std::string(nodeAnim->mNodeName.data) == t_nodeName)
        {
            return nodeAnim;
        }
    }

    return nullptr;
}

void sg::ogl::resource::SkeletalModel::ReadNodeHierarchy(const float t_animationTime, const aiNode* t_node, const glm::mat4& t_parentTransform)
{
    const std::string nodeName(t_node->mName.data);

    const aiAnimation* animation{ m_scene->mAnimations[m_currentAnimation] };
    auto nodeTransform{ mat4_cast(t_node->mTransformation) };

    auto* nodeAnim{ FindNodeAnim(animation, nodeName) };

    if (nodeAnim)
    {
        // interpolate scaling and generate scaling transformation matrix
        const auto scalingVector{ CalcInterpolatedScaling(t_animationTime, nodeAnim) };
        const auto scale{ glm::vec3(scalingVector.x, scalingVector.y, scalingVector.z) };
        const auto scalingMat{ glm::scale(glm::mat4(1.0f), scale) };

        // interpolate rotation and generate rotation transformation matrix
        const auto rotateQuat{ CalcInterpolatedRotation(t_animationTime, nodeAnim) };
        const auto rotation{ quat_cast(rotateQuat) };
        const auto rotationMat{ toMat4(rotation) };

        // interpolate translation and generate translation transformation matrix
        const auto translateVector{ CalcInterpolatedPosition(t_animationTime, nodeAnim) };
        const auto translation{ glm::vec3(translateVector.x, translateVector.y, translateVector.z) };
        const auto translationMat{ translate(glm::mat4(1.0f), translation) };

        // combine transformations
        nodeTransform = translationMat * rotationMat * scalingMat;
    }

    // combine with node transformation with parent transformation
    const auto globalTransform{ t_parentTransform * nodeTransform };

    if (m_boneContainer.find(nodeName) != m_boneContainer.end())
    {
        const auto boneIndex{ m_boneContainer[nodeName] };
        m_boneMatrices[boneIndex].finalWorldTransform = m_globalInverseTransform * globalTransform * m_boneMatrices[boneIndex].offsetMatrix;
    }

    for (auto i{ 0u }; i < t_node->mNumChildren; ++i)
    {
        ReadNodeHierarchy(t_animationTime, t_node->mChildren[i], globalTransform);
    }
}

uint32_t sg::ogl::resource::SkeletalModel::FindScaling(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    SG_OGL_CORE_ASSERT(t_nodeAnim->mNumScalingKeys > 0, "[SkeletalModel::FindScaling()] Invalid number of scaling keys.")

    for (auto i{ 0u }; i < t_nodeAnim->mNumScalingKeys - 1; ++i)
    {
        if (t_animationTime < static_cast<float>(t_nodeAnim->mScalingKeys[i + 1].mTime))
        {
            return i;
        }
    }

    return 0;
}

uint32_t sg::ogl::resource::SkeletalModel::FindRotation(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    SG_OGL_CORE_ASSERT(t_nodeAnim->mNumRotationKeys > 0, "[SkeletalModel::FindRotation()] Invalid number of rotation keys.")

    for (auto i{ 0u }; i < t_nodeAnim->mNumRotationKeys - 1; ++i)
    {
        if (t_animationTime < static_cast<float>(t_nodeAnim->mRotationKeys[i + 1].mTime))
        {
            return i;
        }
    }

    return 0;
}

uint32_t sg::ogl::resource::SkeletalModel::FindPosition(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    SG_OGL_CORE_ASSERT(t_nodeAnim->mNumPositionKeys > 0, "[SkeletalModel::FindPosition()] Invalid number of position keys.")

    for (auto i{ 0u }; i < t_nodeAnim->mNumPositionKeys - 1; ++i)
    {
        if (t_animationTime < static_cast<float>(t_nodeAnim->mPositionKeys[i + 1].mTime))
        {
            return i;
        }
    }

    return 0;
}

aiVector3D sg::ogl::resource::SkeletalModel::CalcInterpolatedPosition(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    if (t_nodeAnim->mNumPositionKeys == 1)
    {
        return t_nodeAnim->mPositionKeys[0].mValue;
    }

    const auto positionIndex{ FindPosition(t_animationTime, t_nodeAnim) };
    const auto nextPositionIndex{ positionIndex + 1 };

    const auto deltaTime{ static_cast<float>(t_nodeAnim->mPositionKeys[nextPositionIndex].mTime - t_nodeAnim->mPositionKeys[positionIndex].mTime) };
    const auto factor{ (t_animationTime - static_cast<float>(t_nodeAnim->mPositionKeys[positionIndex].mTime)) / deltaTime };

    const auto& start{ t_nodeAnim->mPositionKeys[positionIndex].mValue };
    const auto& end{ t_nodeAnim->mPositionKeys[nextPositionIndex].mValue };

    const auto delta{ end - start };

    return start + factor * delta;
}

aiQuaternion sg::ogl::resource::SkeletalModel::CalcInterpolatedRotation(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    if (t_nodeAnim->mNumRotationKeys == 1)
    {
        return t_nodeAnim->mRotationKeys[0].mValue;
    }

    const auto rotationIndex{ FindRotation(t_animationTime, t_nodeAnim) };
    const auto nextRotationIndex{ rotationIndex + 1 };

    const auto deltaTime{ static_cast<float>(t_nodeAnim->mRotationKeys[nextRotationIndex].mTime - t_nodeAnim->mRotationKeys[rotationIndex].mTime) };
    const auto factor{ (t_animationTime - static_cast<float>(t_nodeAnim->mRotationKeys[rotationIndex].mTime)) / deltaTime };

    const auto& startQuat{ t_nodeAnim->mRotationKeys[rotationIndex].mValue };
    const auto& endQuat{ t_nodeAnim->mRotationKeys[nextRotationIndex].mValue };

    return Nlerp(startQuat, endQuat, factor);
}

aiVector3D sg::ogl::resource::SkeletalModel::CalcInterpolatedScaling(const float t_animationTime, const aiNodeAnim* t_nodeAnim)
{
    if (t_nodeAnim->mNumScalingKeys == 1)
    {
        return t_nodeAnim->mScalingKeys[0].mValue;
    }

    const auto scalingIndex{ FindScaling(t_animationTime, t_nodeAnim) };
    const auto nextScalingIndex{ scalingIndex + 1 };

    const auto deltaTime{ static_cast<float>(t_nodeAnim->mScalingKeys[nextScalingIndex].mTime - t_nodeAnim->mScalingKeys[scalingIndex].mTime) };
    const auto factor{ (t_animationTime - static_cast<float>(t_nodeAnim->mScalingKeys[scalingIndex].mTime)) / deltaTime };

    const auto& start{ t_nodeAnim->mScalingKeys[scalingIndex].mValue };
    const auto& end{ t_nodeAnim->mScalingKeys[nextScalingIndex].mValue };

    const auto delta{ end - start };

    return start + factor * delta;
}

aiQuaternion sg::ogl::resource::SkeletalModel::Nlerp(aiQuaternion t_a, aiQuaternion t_b, const float t_blend)
{
    t_a.Normalize();
    t_b.Normalize();

    aiQuaternion result;
    const auto dotProduct{ t_a.x * t_b.x + t_a.y * t_b.y + t_a.z * t_b.z + t_a.w * t_b.w };
    const auto oneMinusBlend{ 1.0f - t_blend };

    if (dotProduct < 0.0f)
    {
        result.x = t_a.x * oneMinusBlend + t_blend * -t_b.x;
        result.y = t_a.y * oneMinusBlend + t_blend * -t_b.y;
        result.z = t_a.z * oneMinusBlend + t_blend * -t_b.z;
        result.w = t_a.w * oneMinusBlend + t_blend * -t_b.w;
    }
    else
    {
        result.x = t_a.x * oneMinusBlend + t_blend * t_b.x;
        result.y = t_a.y * oneMinusBlend + t_blend * t_b.y;
        result.z = t_a.z * oneMinusBlend + t_blend * t_b.z;
        result.w = t_a.w * oneMinusBlend + t_blend * t_b.w;
    }

    return result.Normalize();
}
