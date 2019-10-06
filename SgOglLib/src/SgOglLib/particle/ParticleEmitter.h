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
        using InstancedDataContainer = std::vector<float>;

        /*

        --------------------------------------------------------
        |                       VAO                            |
        --------------------------------------------------------
        |0 2f | -0.5, 0.5 | -0.5, -0.5 | 0.5, 0.5  | 0.5, -0.5 | ---------------- VBO
        --------------------------------------------------------
        |1 4f | mvMat A 0 | mvMat A 1  | mvMat A 2 | mvMat A 3 | instance var ---
        --------------------------------------------------------                |
        |2 4f | mvMat B 0 | mvMat B 1  | mvMat B 2 | mvMat B 3 | instance var   |
        --------------------------------------------------------                |
        |3 4f | mvMat C 0 | mvMat C 1  | mvMat C 2 | mvMat C 3 | instance var   | VBO
        --------------------------------------------------------                |
        |4 4f | mvMat D 0 | mvMat D 1  | mvMat D 2 | mvMat D 3 | instance var   |
        --------------------------------------------------------                |
        |5 4f | texOff  0 | texOff  1  | texOff  2 | texOff  3 | instance var   |
        --------------------------------------------------------                |
        |6 1f | blend   0 | blend   1  | blend   2 | blend   3 | instance var ---
        --------------------------------------------------------
           |            |
        nr of floats    |
          (21)          |
                     Instance number

        */

        static constexpr auto FRAME_TIME{ 0.016f };
        static constexpr auto NUMBER_OF_FLOATS_PER_INSTANCE{ 21 };

        inline static std::vector<float> vertices{
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f
        };

        static constexpr int32_t VERTEX_COUNT{ 4 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ParticleEmitter(
            scene::Scene* t_scene,
            size_t t_maxParticles,
            const std::string& t_texturePath,
            int t_nrOfTextureRows = 1
        );

        ParticleEmitter(const ParticleEmitter& t_other) = delete;
        ParticleEmitter(ParticleEmitter&& t_other) noexcept = delete;
        ParticleEmitter& operator=(const ParticleEmitter& t_other) = delete;
        ParticleEmitter& operator=(ParticleEmitter&& t_other) noexcept = delete;

        ~ParticleEmitter() noexcept = default;

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
        /**
         * @brief Pointer to the parent Scene object.
         */
        scene::Scene* m_scene{ nullptr };

        /**
         * @brief Number of max particles.
         */
        size_t m_maxParticles{ 0 };

        /**
         * @brief A Vertex Array Object.
         */
        VaoUniquePtr m_vao;

        /**
         * @brief A container with all particles.
         */
        ParticleContainer m_particles;

        /**
         * @brief Number of rows in the texture atlas.
         */
        int m_nrOfTextureRows{ 1 };

        /**
         * @brief The texture handle of the texture atlas.
         */
        uint32_t m_textureId{ 0 };

        /**
         * @brief A pointer to the ShaderProgram.
         */
        resource::ShaderProgram* m_shaderProgram{ nullptr };

        /**
         * @brief Vbo Id of instanced data.
         */
        uint32_t m_vbo{ 0 };

        /**
         * @brief A float buffer for instanced data.
         */
        InstancedDataContainer m_instancedData;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitVao();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        glm::vec2 GetTextureOffset(int t_textureIndex) const;
        void PrepareRendering() const;
        static void FinishRendering();
        void UpdateTextureInfo(Particle& t_particle) const;
        void UpdateVbo() const;

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
