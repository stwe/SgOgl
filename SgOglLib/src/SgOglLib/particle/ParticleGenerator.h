#pragma once

#include <vector>
#include <memory>

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::particle
{
    class Particle;

    class ParticleGenerator
    {
    public:
        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using ParticleContainer = std::vector<Particle>;

        static constexpr auto MAX_INSTANCES{ 10000 };
        static constexpr auto NUMBER_OF_FLOATS_PER_INSTANCE{ 21 };

        inline static std::vector<float> VERTICES{ // todo const
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f
        };

        explicit ParticleGenerator(scene::Scene* t_scene);

        ParticleGenerator(const ParticleGenerator& t_other) = delete;
        ParticleGenerator(ParticleGenerator&& t_other) noexcept = delete;
        ParticleGenerator& operator=(const ParticleGenerator& t_other) = delete;
        ParticleGenerator& operator=(ParticleGenerator&& t_other) noexcept = delete;

        ~ParticleGenerator() = default;

        void Update();
        void Render();
        void AddParticle(Particle& t_particle);

    protected:

    private:
        scene::Scene* m_parentScene{ nullptr };
        VaoUniquePtr m_vao;

        ParticleContainer m_particles;
    };
}
