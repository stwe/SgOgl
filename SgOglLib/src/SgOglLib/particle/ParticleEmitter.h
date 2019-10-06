#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <glm/vec2.hpp>

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::resource
{
    class ShaderProgram;
}

namespace sg::ogl::particle
{
    struct Particle;

    class ParticleEmitter
    {
    public:
        using VaoUniquePtr = std::unique_ptr<buffer::Vao>;
        using ParticleContainer = std::vector<Particle>;

        static constexpr auto FRAME_TIME{ 0.016f };

        inline static std::vector<float> vertices{
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleEmitter(
            scene::Scene* t_scene,
            size_t t_maxParticles,
            const std::string& t_texturePath,
            int t_nrOfTextureRows = 1
        );

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        ParticleContainer& GetParticles();

        //-------------------------------------------------
        // Add
        //-------------------------------------------------

        bool AddParticle(Particle& t_particle);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render();

    protected:

    private:
        scene::Scene* m_scene{ nullptr };
        size_t m_maxParticles{ 0 };
        VaoUniquePtr m_vao;
        ParticleContainer m_particles;

        int m_nrOfTextureRows{ 1 };
        uint32_t m_textureId{ 0 };

        resource::ShaderProgram* m_shaderProgram{ nullptr };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitVao() const;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        glm::vec2 GetTextureOffset(int t_textureIndex) const;
        void PrepareRendering();
        static void FinishRendering();
        void UpdateTextureInfo(Particle& t_particle) const;

        //-------------------------------------------------
        // Erase–Remove Idiom
        //-------------------------------------------------

        template<class C, class F>
        void EraseRemoveIf(C&& t_c, F&& t_f)
        {
            auto it{ std::remove_if(std::begin(t_c), std::end(t_c), std::forward<F>(t_f)) };
            t_c.erase(it, std::end(t_c));
        }
    };
}
