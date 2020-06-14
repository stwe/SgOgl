// This file is part of the SgOgl package.
// 
// Filename: ShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "light/PointLight.h"

namespace entt
{
    enum class entity : unsigned;
}

namespace sg::ogl::buffer
{
    class GBufferFbo;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::light
{
    struct DirectionalLight;
}

namespace sg::ogl::terrain
{
    class Terrain;
    class TerrainConfig;
}

namespace sg::ogl::resource
{
    struct Material;
    class Mesh;

    class ShaderProgram
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

        [[nodiscard]] uint32_t GetProgramId() const;

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

        void SetUniform(const std::string& t_uniformName, const std::vector<light::PointLight>& t_pointLights);
        void SetUniform(const std::string& t_uniformName, const std::vector<light::DirectionalLight>& t_directionalLights);

        void SetUniform(const std::string& t_uniformName, const std::map<std::string, std::shared_ptr<light::PointLight>>& t_pointLights);

        void SetUniform(const std::string& t_uniformName, const std::vector<float>& t_container);
        void SetUniform(const std::string& t_uniformName, const std::vector<int32_t>& t_container);
        void SetUniform(const std::string& t_uniformName, const std::vector<glm::mat4>& t_container);

        //-------------------------------------------------
        // To implement
        //-------------------------------------------------

        [[nodiscard]] virtual std::string GetFolderName() const = 0;
        [[nodiscard]] virtual bool IsBuiltIn() const;
        [[nodiscard]] virtual Options GetOptions() const;

        virtual void UpdateUniforms(
            const scene::Scene& t_scene,
            entt::entity t_entity,
            const Mesh& t_currentMesh
        ) {}

        virtual void UpdateUniforms(const scene::Scene& t_scene, entt::entity t_entity, void* t_object) {}

        virtual void UpdateUniforms(
            const scene::Scene& t_scene,
            entt::entity t_entity,
            const Mesh& t_currentMesh,
            const std::vector<light::PointLight>& t_pointLights,
            const std::vector<light::DirectionalLight>& t_directionalLights
        ) {}

        virtual void UpdateUniforms(const scene::Scene& t_scene, const glm::vec3& t_vec3) {}

        virtual void UpdateUniforms(
            const scene::Scene& t_scene,
            const buffer::GBufferFbo& t_gbufferFbo,
            const std::vector<light::PointLight>& t_pointLights,
            const std::vector<light::DirectionalLight>& t_directionalLights
        ) {}

        [[deprecated]] virtual void UpdateUniforms(const terrain::Terrain& t_terrain) {}
        virtual void UpdateUniforms(const terrain::TerrainConfig& t_terrainConfig) {}

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
        std::unordered_map<std::string, int32_t> m_uniforms;
        std::vector<Uniform> m_foundUniforms;

        /*
        Stores array uniforms in the format:
            first: lodMorphArea
                second: lodMorphArea[0]
                second: lodMorphArea[1]
                second: lodMorphArea[2]
                            .
                            .
                            .
         */
        std::unordered_map<std::string, std::vector<std::string>> m_arrayUniformNames;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static uint32_t GenerateShader(int32_t t_shaderType);
        static void CompileShader(uint32_t t_shaderId, const std::string& t_shaderCode);
        static void CheckCompileStatus(uint32_t t_shaderId);

        uint32_t AddShader(const std::string& t_shaderCode, int32_t t_shaderType);

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
