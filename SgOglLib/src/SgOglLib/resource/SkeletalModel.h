// This file is part of the SgOgl package.
// 
// Filename: SkeletalModel.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <string>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::resource
{
    struct VertexBoneData
    {
        static constexpr uint32_t NUM_BONES_PER_VERTEX{ 4 };

        // we have 4 bone ids and 4 weights for each vertex
        std::array<uint32_t, NUM_BONES_PER_VERTEX> ids;
        std::array<float, NUM_BONES_PER_VERTEX> weights;

        void Add(const uint32_t t_boneId, const float t_weight)
        {
            for (uint32_t i{ 0 }; i < NUM_BONES_PER_VERTEX; ++i)
            {
                if (weights[i] == 0.0f)
                {
                    ids[i] = t_boneId;
                    weights[i] = t_weight;

                    return;
                }
            }
        }
    };

    struct BoneMatrix
    {
        aiMatrix4x4 offsetMatrix;
        aiMatrix4x4 finalWorldTransform;
    };

    class Mesh;

    class SkeletalModel
    {
    public:
        using VertexContainer = std::vector<float>;
        using IndexContainer = std::vector<uint32_t>;
        using VertexBones = std::vector<VertexBoneData>;
        using TextureContainer = std::vector<uint32_t>;
        using MeshUniquePtr = std::unique_ptr<Mesh>;
        using MeshSharedPtr = std::shared_ptr<Mesh>;
        using MeshContainer = std::vector<MeshSharedPtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SkeletalModel() = delete;

        SkeletalModel(std::string t_fullFilePath, Application* t_application, unsigned int t_pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

        SkeletalModel(const SkeletalModel& t_other) = delete;
        SkeletalModel(SkeletalModel&& t_other) noexcept = delete;
        SkeletalModel& operator=(const SkeletalModel& t_other) = delete;
        SkeletalModel& operator=(SkeletalModel&& t_other) noexcept = delete;

        ~SkeletalModel() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const MeshContainer& GetMeshes() const noexcept;

        //-------------------------------------------------
        // Transform
        //-------------------------------------------------

        void BoneTransform(double t_timeInSec, std::vector<aiMatrix4x4>& t_transforms);

    protected:

    private:
        Application* m_application{ nullptr };

        MeshContainer m_meshes;

        std::string m_fullFilePath;
        std::string m_directory;

        Assimp::Importer m_importer;
        const aiScene* m_scene{ nullptr };
        aiMatrix4x4 m_globalInverseTransform;
        float m_ticksPerSecond{ 0.0f };
        std::map<std::string, uint32_t> m_boneMapping;
        uint32_t m_numBones{ 0 };
        std::vector<BoneMatrix> m_boneMatrices;

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        static void ShowNodeName(aiNode* t_node);

        void LoadModel(unsigned int t_pFlags);
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene);
        TextureContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static const aiNodeAnim* FindNodeAnim(const aiAnimation* t_animation, const std::string& t_nodeName);
        void ReadNodeHierarchy(float t_animationTime, const aiNode* t_node, const aiMatrix4x4& t_parentTransform);

        static uint32_t FindScaling(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static uint32_t FindRotation(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static uint32_t FindPosition(float t_animationTime, const aiNodeAnim* t_nodeAnim);

        static aiVector3D CalcInterpolatedPosition(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static aiQuaternion CalcInterpolatedRotation(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static aiVector3D CalcInterpolatedScaling(float t_animationTime, const aiNodeAnim* t_nodeAnim);

        static aiQuaternion Nlerp(aiQuaternion t_a, aiQuaternion t_b, float t_blend);
    };
}
