#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "ShaderUtil.h"

void sg::ogl::resource::ShaderManager::AddShaderProgram(const std::string& t_folder)
{
    // todo: Versuch Doppeleintrag -> Exception

    auto shaderProgram{ std::make_unique<ShaderProgram>() };

#if defined(_WIN64) && defined(_MSC_VER)

    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/Vertex.vert"));
    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/Fragment.frag"));

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/" + t_folder + "/Vertex.vert"));
    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/" + t_folder + "/Fragment.frag"));

#else

#error Unsupported platform or unsupported compiler!

#endif

    shaderProgram->LinkAndValidateProgram();
    shaderProgram->AddAllFoundUniforms();

    m_shaderPrograms.emplace(t_folder, std::move(shaderProgram));
}

sg::ogl::resource::ShaderManager::ShaderPrograms& sg::ogl::resource::ShaderManager::GetShaderPrograms() noexcept
{
    return m_shaderPrograms;
}

const sg::ogl::resource::ShaderManager::ShaderPrograms& sg::ogl::resource::ShaderManager::GetShaderPrograms() const noexcept
{
    return m_shaderPrograms;
}

sg::ogl::resource::ShaderManager::ShaderProgramUniquePtr& sg::ogl::resource::ShaderManager::GetShaderProgram(const std::string& t_name)
{
    if (m_shaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetShaderProgram()] Shader program " + t_name + " not exist.");
    }

    return m_shaderPrograms.at(t_name);
}

void sg::ogl::resource::ShaderManager::CleanUp()
{
    for (auto& shaderProgram : m_shaderPrograms)
    {
        shaderProgram.second->CleanUp();
    }
}
