#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "Log.h"
#include "SgOglException.h"
#include "OpenGl.h"
#include "ShaderUtil.h"
#include "Material.h"
#include "light/DirectionalLight.h"
#include "light/PointLight.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ShaderProgram::ShaderProgram()
{
    m_programId = glCreateProgram();
    SG_OGL_CORE_ASSERT(m_programId, "[ShaderProgram::ShaderProgram()] Error while creating a new Shader Program.")

    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::ShaderProgram()] A new ShaderProgram was created. Id: {}", m_programId);
}

sg::ogl::resource::ShaderProgram::~ShaderProgram() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::~ShaderProgram()] Destruct ShaderProgram.");
    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::resource::ShaderProgram::GetProgramId() const
{
    return m_programId;
}

//-------------------------------------------------
// Add shader types
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::AddVertexShader(const std::string& t_shaderCode)
{
    m_vertexShaderId = AddShader(t_shaderCode, GL_VERTEX_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddVertexShader()] A new vertex shader was added. Id: {}", m_vertexShaderId);
}

void sg::ogl::resource::ShaderProgram::AddTessellationControlShader(const std::string& t_shaderCode)
{
    m_tessellationControlShaderId = AddShader(t_shaderCode, GL_TESS_CONTROL_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddTessellationControlShader()] A new tessellation control shader was added. Id: {}", m_tessellationControlShaderId);
}

void sg::ogl::resource::ShaderProgram::AddTessellationEvaluationShader(const std::string& t_shaderCode)
{
    m_tessellationEvaluationShaderId = AddShader(t_shaderCode, GL_TESS_EVALUATION_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddTessellationEvaluationShader()] A new tessellation evaluation shader was added. Id: {}", m_tessellationEvaluationShaderId);
}

void sg::ogl::resource::ShaderProgram::AddGeometryShader(const std::string& t_shaderCode)
{
    m_geometryShaderId = AddShader(t_shaderCode, GL_GEOMETRY_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddGeometryShader()] A new geometry shader was added. Id: {}", m_geometryShaderId);
}

void sg::ogl::resource::ShaderProgram::AddFragmentShader(const std::string& t_shaderCode)
{
    m_fragmentShaderId = AddShader(t_shaderCode, GL_FRAGMENT_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddFragmentShader()] A new fragment shader was added. Id: {}", m_fragmentShaderId);
}

void sg::ogl::resource::ShaderProgram::AddComputeShader(const std::string& t_shaderCode)
{
    m_computeShaderId = AddShader(t_shaderCode, GL_COMPUTE_SHADER);
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddComputeShader()] A new compute shader was added. Id: {}", m_computeShaderId);
}

//-------------------------------------------------
// Link
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::LinkAndValidateProgram() const
{
    SG_OGL_CORE_ASSERT(m_programId, "[ShaderProgram::LinkAndValidateProgram()] Invalid Shader Program Id.")

    // link our program
    glLinkProgram(m_programId);

    // error handling
    auto isLinked{ GL_FALSE };
    glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

        CleanUp();

        std::string log;
        for (const auto& value : infoLog)
        {
            log.push_back(value);
        }

        throw SG_OGL_EXCEPTION("[ShaderProgram::LinkAndValidate()] Error while linking shader program. Log: " + log);
    }

    // cleanup: always detach shaders after a successful link
    if (m_vertexShaderId != 0)
    {
        glDetachShader(m_programId, m_vertexShaderId);
    }

    if (m_tessellationControlShaderId != 0)
    {
        glDetachShader(m_programId, m_tessellationControlShaderId);
    }

    if (m_tessellationEvaluationShaderId != 0)
    {
        glDetachShader(m_programId, m_tessellationEvaluationShaderId);
    }

    if (m_geometryShaderId != 0)
    {
        glDetachShader(m_programId, m_geometryShaderId);
    }

    if (m_fragmentShaderId != 0)
    {
        glDetachShader(m_programId, m_fragmentShaderId);
    }

    if (m_computeShaderId != 0)
    {
        glDetachShader(m_programId, m_computeShaderId);
    }

    // validate our program
    glValidateProgram(m_programId);

    auto isValidated{ GL_FALSE };
    glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &isValidated);
    if (isValidated == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

        CleanUp();

        std::string log;
        for (const auto& value : infoLog)
        {
            log.push_back(value);
        }

        throw SG_OGL_EXCEPTION("[ShaderProgram::LinkAndValidate()] Shader Program validation error. Log: " + log);
    }
}

//-------------------------------------------------
// Add uniforms
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::AddUniform(const std::string& t_uniformName)
{
    auto uniformId{ glGetUniformLocation(m_programId, t_uniformName.c_str()) };
    m_uniforms.emplace(t_uniformName, uniformId);
}

void sg::ogl::resource::ShaderProgram::AddDirectionalLightUniform(const std::string& t_uniformName)
{
    AddUniform(t_uniformName + ".direction");
    AddUniform(t_uniformName + ".diffuseIntensity");
    AddUniform(t_uniformName + ".specularIntensity");
}

void sg::ogl::resource::ShaderProgram::AddPointLightUniform(const std::string& t_uniformName)
{
    AddUniform(t_uniformName + ".position");
    AddUniform(t_uniformName + ".ambientIntensity");
    AddUniform(t_uniformName + ".diffuseIntensity");
    AddUniform(t_uniformName + ".specularIntensity");
    AddUniform(t_uniformName + ".constant");
    AddUniform(t_uniformName + ".linear");
    AddUniform(t_uniformName + ".quadratic");
}

void sg::ogl::resource::ShaderProgram::AddMaterialUniform(const std::string& t_uniformName)
{
    AddUniform(t_uniformName + ".diffuseColor");
    AddUniform(t_uniformName + ".specularColor");
    AddUniform(t_uniformName + ".shininess");
    AddUniform(t_uniformName + ".hasDiffuseMap");
    AddUniform(t_uniformName + ".hasSpecularMap");
}

void sg::ogl::resource::ShaderProgram::AddAllFoundUniforms()
{
    for (const auto& uniform : m_foundUniforms)
    {
        if (uniform.isStruct)
        {
            if (uniform.type == "DirectionalLight")
            {
                AddDirectionalLightUniform(uniform.name);
            }

            if (uniform.type == "PointLight")
            {
                AddPointLightUniform(uniform.name);
            }

            if (uniform.type == "Material")
            {
                AddMaterialUniform(uniform.name);
            }
        }
        else
        {
            AddUniform(uniform.name);
        }
    }

    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::AddAllFoundUniforms()] {} found uniforms have been added to shader program. Id: {}", m_foundUniforms.size(), m_programId);
}

//-------------------------------------------------
// Bind / Unbind
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::Bind() const
{
    SG_OGL_CORE_ASSERT(m_programId, "[ShaderProgram::Bind()] Invalid ShaderProgram Id.")
    glUseProgram(m_programId);
}

void sg::ogl::resource::ShaderProgram::Unbind()
{
    glUseProgram(0);
}

//-------------------------------------------------
// Set uniforms
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const int32_t t_value)
{
    glUniform1i(GetUniformLocation(t_uniformName), t_value);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const float t_value)
{
    glUniform1f(GetUniformLocation(t_uniformName), t_value);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const bool t_value)
{
    // if value == true load 1 else 0 as float
    glUniform1f(GetUniformLocation(t_uniformName), t_value ? 1.0f : 0.0f);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const glm::vec2& t_value)
{
    glUniform2f(GetUniformLocation(t_uniformName), t_value.x, t_value.y);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const glm::vec3& t_value)
{
    glUniform3f(GetUniformLocation(t_uniformName), t_value.x, t_value.y, t_value.z);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const glm::vec4& t_value)
{
    glUniform4f(GetUniformLocation(t_uniformName), t_value.x, t_value.y, t_value.z, t_value.w);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const glm::mat4& t_value)
{
    glUniformMatrix4fv(GetUniformLocation(t_uniformName), 1, GL_FALSE, value_ptr(t_value));
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const glm::mat3& t_value)
{
    glUniformMatrix3fv(GetUniformLocation(t_uniformName), 1, GL_FALSE, value_ptr(t_value));
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const light::DirectionalLight& t_directionalLight)
{
    SetUniform(t_uniformName + ".direction", t_directionalLight.direction);
    SetUniform(t_uniformName + ".diffuseIntensity", t_directionalLight.diffuseIntensity);
    SetUniform(t_uniformName + ".specularIntensity", t_directionalLight.specularIntensity);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const light::PointLight& t_pointLight)
{
    SetUniform(t_uniformName + ".position", t_pointLight.position);
    SetUniform(t_uniformName + ".ambientIntensity", t_pointLight.ambientIntensity);
    SetUniform(t_uniformName + ".diffuseIntensity", t_pointLight.diffuseIntensity);
    SetUniform(t_uniformName + ".specularIntensity", t_pointLight.specularIntensity);
    SetUniform(t_uniformName + ".constant", t_pointLight.constant);
    SetUniform(t_uniformName + ".linear", t_pointLight.linear);
    SetUniform(t_uniformName + ".quadratic", t_pointLight.quadratic);
}

void sg::ogl::resource::ShaderProgram::SetUniform(const std::string& t_uniformName, const Material& t_material)
{
    SetUniform(t_uniformName + ".diffuseColor", t_material.kd);
    SetUniform(t_uniformName + ".specularColor", t_material.ks);
    SetUniform(t_uniformName + ".shininess", t_material.ns);
    SetUniform(t_uniformName + ".hasDiffuseMap", t_material.HasDiffuseMap());
    SetUniform(t_uniformName + ".hasSpecularMap", t_material.HasSpecularMap());
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

uint32_t sg::ogl::resource::ShaderProgram::GenerateShader(const int32_t t_shaderType)
{
    const auto shaderId{ glCreateShader(t_shaderType) };
    return shaderId;
}

void sg::ogl::resource::ShaderProgram::CompileShader(const uint32_t t_shaderId, const std::string& t_shaderCode)
{
    auto shaderSrc{ t_shaderCode.c_str() };
    glShaderSource(t_shaderId, 1, &shaderSrc, nullptr);
    glCompileShader(t_shaderId);
}

void sg::ogl::resource::ShaderProgram::CheckCompileStatus(const uint32_t t_shaderId)
{
    auto isCompiled{ GL_FALSE };
    glGetShaderiv(t_shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetShaderiv(t_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(t_shaderId, maxLength, &maxLength, &errorLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(t_shaderId);

        std::string log;
        for (const auto& value : errorLog)
        {
            log.push_back(value);
        }

        throw SG_OGL_EXCEPTION("[ShaderProgram::CheckCompileStatus()] Error while compiling shader program. Log: " + log);
    }
}

uint32_t sg::ogl::resource::ShaderProgram::AddShader(const std::string& t_shaderCode, const int32_t t_shaderType)
{
    // generate shader && compile shader code
    const auto shaderId{ GenerateShader(t_shaderType) };
    CompileShader(shaderId, t_shaderCode);
    CheckCompileStatus(shaderId);
    glAttachShader(m_programId, shaderId);

    // find && store struct names
    std::string structKeyword{ "struct" };
    const auto structKeywordLength{ structKeyword.length() };

    std::vector<size_t> structPositions;
    ShaderUtil::FindAllOccurances(structPositions, t_shaderCode, structKeyword);

    for (const auto& position : structPositions)
    {
        const auto begin{ position + structKeywordLength + 1 };
        const auto end{ t_shaderCode.find_first_of('{', begin) };
        auto structName{ t_shaderCode.substr(begin, end - begin) };
        structName.erase(std::remove(structName.begin(), structName.end(), '\n'), structName.end());

        m_uniformStructs.push_back(structName);
    }

    // find && store uniform names
    std::string uniformKeyword{ "uniform" };
    const auto uniformKeywordLength{ uniformKeyword.length() };

    std::vector<size_t> uniformPositions;
    ShaderUtil::FindAllOccurances(uniformPositions, t_shaderCode, uniformKeyword);

    for (const auto& position : uniformPositions)
    {
        const auto begin{ position + uniformKeywordLength + 1 };
        const auto end{ t_shaderCode.find_first_of(';', begin) };
        const auto uniformLine{ t_shaderCode.substr(begin, end - begin) };

        const auto uniformNamePos{ uniformLine.find_first_of(' ') + 1 };
        const auto uniformName{ uniformLine.substr(uniformNamePos, uniformLine.length()) };

        const auto uniformType{ uniformLine.substr(0, uniformNamePos - 1) };

        Uniform uniform;
        uniform.type = uniformType;

        for (const auto& uniformStruct : m_uniformStructs)
        {
            if (uniform.type == uniformStruct)
            {
                uniform.isStruct = true;
            }
        }

        const auto uniformArrayStartPos{ uniformName.find_first_of('[') };
        if (uniformArrayStartPos != std::string::npos)
        {
            const auto uniformArrayEndPos{ uniformName.find_first_of(']') };
            const auto number{ uniformName.substr(uniformArrayStartPos + 1, (uniformArrayEndPos - 1) - uniformArrayStartPos) };
            const auto iNumber{ std::stoi(number) };
            const auto newUniformName{ uniformName.substr(0, uniformArrayStartPos) };

            for (auto i{ 0 }; i < iNumber; ++i)
            {
                uniform.name = newUniformName + "[" + std::to_string(i) + "]";

                m_foundUniforms.push_back(uniform);
            }
        }
        else
        {
            uniform.name = uniformName;

            m_foundUniforms.push_back(uniform);
        }
    }

    return shaderId;
}

int32_t sg::ogl::resource::ShaderProgram::GetUniformLocation(const std::string& t_uniformName)
{
    if (m_uniforms.count(t_uniformName) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderProgram::GetUniformLocation()] Uniform " + t_uniformName + " location not exist.");
    }

    return m_uniforms.at(t_uniformName);
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::CleanUp() const
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Start the OpenGL clean up process for ShaderProgram. Id: {}", m_programId);

    Unbind();

    if (m_vertexShaderId)
    {
        glDeleteShader(m_vertexShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Vertex shader was deleted. Id: {}", m_vertexShaderId);
    }

    if (m_tessellationControlShaderId)
    {
        glDeleteShader(m_tessellationControlShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Tessellation control shader was deleted. Id: {}", m_tessellationControlShaderId);
    }

    if (m_tessellationEvaluationShaderId)
    {
        glDeleteShader(m_tessellationEvaluationShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Tessellation evaluation shader was deleted. Id: {}", m_tessellationEvaluationShaderId);
    }

    if (m_geometryShaderId)
    {
        glDeleteShader(m_geometryShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Geometry shader was deleted. Id: {}", m_geometryShaderId);
    }

    if (m_fragmentShaderId)
    {
        glDeleteShader(m_fragmentShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Fragment shader was deleted. Id: {}", m_fragmentShaderId);
    }

    if (m_computeShaderId)
    {
        glDeleteShader(m_computeShaderId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Compute shader was deleted. Id: {}", m_computeShaderId);
    }

    if (m_programId)
    {
        glDeleteProgram(m_programId);
        SG_OGL_CORE_LOG_DEBUG("[ShaderProgram::CleanUp()] Shader program was deleted. Id: {}", m_programId);
    }
}
