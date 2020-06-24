// This file is part of the SgOgl package.
// 
// Filename: ParticleSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <vector>
#include <memory>
#include "Particle.h"

namespace sg::ogl::resource
{
    class Mesh;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::particle
{
    class ParticleSystem
    {
    public:
        using ParticleContainer = std::vector<Particle>;
        using InstancedDataContainer = std::vector<float>;
        using MeshUniquePtr = std::unique_ptr<resource::Mesh>;

        /**
         * @brief Number of elements in the particles vector.
         */
        static constexpr uint32_t NR_OF_ELEMENTS{ 1000 };

        static constexpr auto GRAVITY{ -9.81f };
        static constexpr auto NUMBER_OF_FLOATS_PER_INSTANCE{ 21 };

        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        ParticleContainer particles;
        bool instancing{ true };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleSystem(scene::Scene* t_scene, uint32_t t_textureId, int t_textureRows);

        ParticleSystem(
            scene::Scene* t_scene,
            uint32_t t_textureId,
            int t_textureRows,
            float t_particlesPerSecond,
            float t_speed,
            float t_gravityEffect,
            float t_lifeTime,
            float t_maxScale
        );

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] uint32_t GetTextureId() const;
        [[nodiscard]] int GetTextureRows() const;

        [[nodiscard]] const resource::Mesh& GetMesh() const noexcept;
        [[nodiscard]] resource::Mesh& GetMesh() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetParticlesPerSecond(float t_particlesPerSecond);
        void SetSpeed(float t_speed);
        void SetGravityEffect(float t_gravityEffect);
        void SetLifeTime(float t_lifeTime);
        void SetMaxScale(float t_maxScale);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void GenerateParticles(double t_dt, const glm::vec3& t_systemCenter);

        void Update(double t_dt);
        void Render();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        [[nodiscard]] glm::vec2 GetTextureOffset(int t_textureIndex) const;

    protected:

    private:
        uint32_t m_containerIndex{ NR_OF_ELEMENTS - 1 };

        scene::Scene* m_scene{ nullptr };

        uint32_t m_textureId{ 0 };
        int m_textureRows{ 1 };
        int m_nrTextures{ 1 };

        float m_particlesPerSecond{ 25.0f };
        float m_speed{ 25.0f };
        float m_gravityEffect{ 0.5f };
        float m_lifeTime{ 4.0f };
        float m_maxScale{ 1.0f };

        /**
         * @brief Vbo Id of instanced data.
         */
        uint32_t m_vboId{ 0 };

        /**
         * @brief A Mesh for instanced rendering.
         */
        MeshUniquePtr m_mesh;

        /**
         * @brief A float buffer for instanced data.
         */
        InstancedDataContainer m_instancedData;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Emit
        //-------------------------------------------------

        void Emit(const glm::vec3& t_systemCenter);
    };
}
