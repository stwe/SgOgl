#pragma once

#include <vector>
#include "Core.h"

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::particle
{
    struct Particle;

    class SG_OGL_API ParticleEmitter
    {
    public:
        static constexpr auto MAX_PARTICLES{ 10000 };
        static constexpr auto NEW_PARTICLES{ 100.0 };

        inline static const std::vector<float> VERTEX_BUFFER_DATA{
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
             0.5f,  0.5f, 0.0f
        };

        using ParticleContainer = std::vector<Particle>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleEmitter() = delete;

        explicit ParticleEmitter(scene::Scene* t_scene);

        ParticleEmitter(const ParticleEmitter& t_other) = delete;
        ParticleEmitter(ParticleEmitter&& t_other) noexcept = delete;
        ParticleEmitter& operator=(const ParticleEmitter& t_other) = delete;
        ParticleEmitter& operator=(ParticleEmitter&& t_other) noexcept = delete;

        ~ParticleEmitter() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(double t_dt);
        void Render();

    protected:

    private:
        scene::Scene* m_parentScene{ nullptr };

        ParticleContainer m_particles;

        int m_idxOfLastDeadParticle{ 0 };
        int m_particlesCount{ 0 };

        uint32_t m_vaoId{ 0 };
        uint32_t m_vboVertices{ 0 };
        uint32_t m_vboPositions{ 0 };
        uint32_t m_vboColors{ 0 };

        float* m_positions{ nullptr };
        float* m_colors{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void InitBuffers();

        int GetFirstDeadParticle();
        void SortParticles();
    };
}
