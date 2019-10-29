#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Core.h"

namespace entt {
    enum class entity : unsigned;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::light
{
    struct DirectionalLight;
    struct PointLight;
}

namespace sg::ogl::resource
{
    struct Material;
    class Mesh;

    class SG_OGL_API ShaderProgram
    {
    public:
        using Options = uint8_t;

        enum Flags : Options
        {
            VERTEX_SHADER = 1,
            TESSELLATION_CONTROL_SHADER = 2,
            TESSELLATION_EVALUATION_SHADER = 4,
            GEOMETRY_SHADER = 8,
            FRAGMENT_SHADER = 16
        };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderProgram();

        ShaderProgram(const ShaderProgram& t_other) = delete;
        ShaderProgram(ShaderProgram&& t_other) noexcept = delete;
        ShaderProgram& operator=(const ShaderProgram& t_other) = delete;
        ShaderProgram& operator=(ShaderProgram&& t_other) noexcept = delete;

        virtual ~ShaderProgram() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetProgramId() const;

        //-------------------------------------------------
        // Add shader types
        //-------------------------------------------------

        void AddVertexShader(const std::string& t_shaderCode);
        void AddTessellationControlShader(const std::string& t_shaderCode);
        void AddTessellationEvaluationShader(const std::string& t_shaderCode);
        void AddGeometryShader(const std::string& t_shaderCode);
        void AddFragmentShader(const std::string& t_shaderCode);
        void AddComputeShader(const std::string& t_shaderCode);

        //-------------------------------------------------
        // Link
        //-------------------------------------------------

        void LinkAndValidateProgram() const;

        //-------------------------------------------------
        // Add uniforms
        //-------------------------------------------------

        void AddUniform(const std::string& t_uniformName);
        void AddDirectionalLightUniform(const std::string& t_uniformName);
        void AddPointLightUniform(const std::string& t_uniformName);
        void AddMaterialUniform(const std::string& t_uniformName);
        void AddAllFoundUniforms();

        //-------------------------------------------------
        // Bind / Unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Set uniforms
        //-------------------------------------------------

        void SetUniform(const std::string& t_uniformName, int32_t t_value);
        void SetUniform(const std::string& t_uniformName, float t_value);
        void SetUniform(const std::string& t_uniformName, bool t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec2& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec3& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::vec4& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::mat4& t_value);
        void SetUniform(const std::string& t_uniformName, const glm::mat3& t_value);
        void SetUniform(const std::string& t_uniformName, const light::DirectionalLight& t_directionalLight);
        void SetUniform(const std::string& t_uniformName, const light::PointLight& t_pointLight);
        void SetUniform(const std::string& t_uniformName, const Material& t_material);

        //-------------------------------------------------
        // To implement
        //-------------------------------------------------

        virtual std::string GetFolderName() = 0;
        virtual Options GetOptions() const;
        virtual void UpdateUniforms(const scene::Scene& t_scene, entt::entity t_entity, const Mesh& t_currentMesh) = 0;

    protected:

    private:
        struct Uniform
        {
            bool isStruct{ false };
            std::string type;
            std::string name;
        };

        uint32_t m_programId{ 0 };

        uint32_t m_vertexShaderId{ 0 };
        uint32_t m_tessellationControlShaderId{ 0 };
        uint32_t m_tessellationEvaluationShaderId{ 0 };
        uint32_t m_geometryShaderId{ 0 };
        uint32_t m_fragmentShaderId{ 0 };
        uint32_t m_computeShaderId{ 0 };

        std::vector<std::string> m_uniformStructs;
        std::map<std::string, int32_t> m_uniforms;
        std::vector<Uniform> m_foundUniforms;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static uint32_t GenerateShader(int32_t t_shaderType);
        static void CompileShader(uint32_t t_shaderId, const std::string& t_shaderCode);
        static void CheckCompileStatus(uint32_t t_shaderId);

        uint32_t AddShader(const std::string& t_shaderCode, int32_t t_shaderType);

        int32_t GetUniformLocation(const std::string& t_uniformName);

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
