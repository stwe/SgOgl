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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    class Mesh;

    //-------------------------------------------------
    // Converting between Assimp and glm
    //-------------------------------------------------

    static glm::vec3 vec3_cast(const aiVector3D& t_v) { return glm::vec3(t_v.x, t_v.y, t_v.z); }
    static glm::vec2 vec2_cast(const aiVector3D& t_v) { return glm::vec2(t_v.x, t_v.y); }
    static glm::quat quat_cast(const aiQuaternion& t_q) { return glm::quat(t_q.w, t_q.x, t_q.y, t_q.z); }
    static glm::mat4 mat4_cast(const aiMatrix4x4& t_m) { return transpose(glm::make_mat4(&t_m.a1)); }
    static glm::mat4 mat4_cast(const aiMatrix3x3& t_m) { return transpose(glm::make_mat3(&t_m.a1)); }

    //-------------------------------------------------
    // Per-vertex bone Ids and weights
    //-------------------------------------------------

    struct VertexBoneData
    {
        static constexpr uint32_t NUM_BONES_PER_VERTEX{ 4 };

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

    //-------------------------------------------------
    // Information about a single bone
    //-------------------------------------------------

    struct BoneMatrix
    {
        glm::mat4 offsetMatrix{ glm::mat4(0.0f) };
        glm::mat4 finalWorldTransform{ glm::mat4(0.0f) };
    };

    //-------------------------------------------------
    // Skeletal model
    //-------------------------------------------------

    class SkeletalModel
    {
    public:
        using VertexContainer = std::vector<float>;
        using IndexContainer = std::vector<uint32_t>;

        using BoneContainer = std::map<std::string, uint32_t>;
        using VertexBonesContainer = std::vector<VertexBoneData>;

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
        [[nodiscard]] uint32_t GetNumberOfAnimations() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentAnimation(uint32_t t_animation);

        //-------------------------------------------------
        // Transform
        //-------------------------------------------------

        void BoneTransform(double t_timeInSec, std::vector<glm::mat4>& t_transforms);

    protected:

    private:
        Application* m_application{ nullptr };

        MeshContainer m_meshes;

        std::string m_fullFilePath;
        std::string m_directory;

        /**
         * @brief Instance of the Importer class to call ReadFile().
         */
        Assimp::Importer m_importer;

        /**
         * @brief The root structure of the imported data.
         */
        const aiScene* m_scene{ nullptr };

        /**
         * @brief The current animation index.
         */
        uint32_t m_currentAnimation{ 0 };

        /**
         * @brief Root inverse transform matrix.
         */
        glm::mat4 m_globalInverseTransform{ glm::mat4(1.0f) };

        /**
         * @brief Stores the name for each bone Id.
         */
        BoneContainer m_boneContainer;

        /**
         * @brief Number of bones.
         */
        uint32_t m_numBones{ 0 };

        /**
         * @brief Bone matrices container.
         */
        std::vector<BoneMatrix> m_boneMatrices;

        //-------------------------------------------------
        // Load Model
        //-------------------------------------------------

        void LoadModel(unsigned int t_pFlags);
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        MeshUniquePtr ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene);
        TextureContainer LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static const aiNodeAnim* FindNodeAnim(const aiAnimation* t_animation, const std::string& t_nodeName);
        void ReadNodeHierarchy(float t_animationTime, const aiNode* t_node, const glm::mat4& t_parentTransform);

        static uint32_t FindScaling(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static uint32_t FindRotation(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static uint32_t FindPosition(float t_animationTime, const aiNodeAnim* t_nodeAnim);

        static aiVector3D CalcInterpolatedPosition(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static aiQuaternion CalcInterpolatedRotation(float t_animationTime, const aiNodeAnim* t_nodeAnim);
        static aiVector3D CalcInterpolatedScaling(float t_animationTime, const aiNodeAnim* t_nodeAnim);

        static aiQuaternion Nlerp(aiQuaternion t_a, aiQuaternion t_b, float t_blend);
    };
}
