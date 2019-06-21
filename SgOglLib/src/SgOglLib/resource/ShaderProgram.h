#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

namespace sg::ogl::resource
{
    class ShaderProgram
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ShaderProgram();

        ShaderProgram(const ShaderProgram& t_other) = delete;
        ShaderProgram(ShaderProgram&& t_other) noexcept = delete;
        ShaderProgram& operator=(const ShaderProgram& t_other) = delete;
        ShaderProgram& operator=(ShaderProgram&& t_other) noexcept = delete;

        ~ShaderProgram() = default;

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

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;

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
    };
}
