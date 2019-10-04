#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sg::ogl::particle
{
    class ParticleEmitter;

    class Particle
    {
    public:
        explicit Particle(ParticleEmitter* t_particleEmitter);

        glm::vec3 position{ glm::vec3(0.0f) };
        glm::vec3 velocity{ glm::vec3(0.0f) };
        glm::vec4 color{ glm::vec4(1.0f) };

        float rotation{ 0.0f };
        float scale{ 1.0f };

        float lifetime{ 5.0f };
        bool life{ true };

        void Update();
        void Render() const;

    protected:

    private:
        ParticleEmitter* m_particleEmitter{ nullptr };
    };
}
