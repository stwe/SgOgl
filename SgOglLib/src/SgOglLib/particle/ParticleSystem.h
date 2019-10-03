#pragma once

#include <glm/vec3.hpp>
#include <memory>

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::particle
{
    class ParticleGenerator;

    class ParticleSystem
    {
    public:
        using ParticleGeneratorUniquePtr = std::unique_ptr<ParticleGenerator>;

        ParticleSystem(
            scene::Scene* t_scene,
            float t_pps,
            float t_speed,
            float t_gravityComplient,
            float t_lifeLength
        );

        void GenerateParticles(const glm::vec3& t_systemCenter) const;

        void Update();
        void Render();

    protected:

    private:
        float m_pps;
        float m_speed;
        float m_gravityComplient;
        float m_lifeLength;

        ParticleGeneratorUniquePtr m_particleGenerator;

        void EmitParticle(const glm::vec3& t_center) const;
    };
}
