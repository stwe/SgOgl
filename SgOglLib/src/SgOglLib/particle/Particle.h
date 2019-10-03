#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sg::ogl::particle
{
    class ParticleGenerator;

    class Particle
    {
    private:
        ParticleGenerator* m_parentParticleGenerator{ nullptr };
        float m_elapsedTime{ 0.0f };

    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Particle() = default;

        explicit Particle(ParticleGenerator* t_parentParticleGenerator);

        Particle(
            ParticleGenerator* t_parentParticleGenerator,
            const glm::vec3& t_position,
            const glm::vec3& t_velocity,
            float t_gravity,
            float t_lifeLength,
            float t_rotation,
            float t_scale
        );

        Particle(const Particle& t_other) = default;
        Particle(Particle&& t_other) noexcept = default;
        Particle& operator=(const Particle& t_other) = default;
        Particle& operator=(Particle&& t_other) noexcept = default;

        ~Particle() noexcept = default;

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };

        float gravity{ 0.0f };
        float lifeLength{ 0.0f };
        float rotation{ 0.0f };
        float scale{ 0.0f };

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        bool Update(double t_dt);

    protected:

    };
}
