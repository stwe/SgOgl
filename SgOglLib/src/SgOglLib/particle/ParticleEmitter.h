#pragma once

#include <vector>
#include <algorithm>
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

    class ParticleEmitter
    {
    public:
        ParticleEmitter(scene::Scene* t_scene, size_t t_maxParticles);

        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using ParticleContainer = std::vector<Particle>;

        static constexpr auto FRAME_TIME{ 0.016f };

        inline static std::vector<float> vertices{
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f
        };

        scene::Scene* GetParentScene() const;
        VaoUniquePtr& GetVao();
        ParticleContainer& GetParticles();
        uint32_t GetTextureId() const { return m_textureId; }

        bool AddParticle(Particle& t_particle);

        void Update();
        void Render();

    protected:

    private:
        scene::Scene* m_scene{ nullptr };
        size_t m_maxParticles{ 0 };
        VaoUniquePtr m_vao;
        ParticleContainer m_particles;
        uint32_t m_textureId{ 0 };

        bool m_emptyContainer{ false };

        void Init();

        template<class C, class F>
        void EraseRemoveIf(C&& t_c, F&& t_f)
        {
            auto it{ std::remove_if(std::begin(t_c), std::end(t_c), std::forward<F>(t_f)) };
            t_c.erase(it, std::end(t_c));
        }
    };
}
