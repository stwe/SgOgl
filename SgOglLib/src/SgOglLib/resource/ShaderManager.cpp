#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "ShaderUtil.h"
#include "Log.h"

//-------------------------------------------------
// Custom Deleter
//-------------------------------------------------

void sg::ogl::resource::DeleteShaderProgram::operator()(ShaderProgram* t_shaderProgram) const
{
    SG_OGL_CORE_LOG_DEBUG("[DeleteShaderProgram::operator()] Delete ShaderProgram Id: {}.", t_shaderProgram->GetProgramId());
    delete t_shaderProgram;
}

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ShaderManager::ShaderManager()
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::ShaderManager()] Create ShaderManager.");
}

sg::ogl::resource::ShaderManager::~ShaderManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::~ShaderManager()] Destruct ShaderManager.");
}

//-------------------------------------------------
// Add shader program
//-------------------------------------------------

void sg::ogl::resource::ShaderManager::AddShaderProgram(const std::string& t_folder, const bool t_loadGeometryShader)
{
    // todo: auf Doppeleintraege prüfen
    // todo: Pfad -> Config

    ShaderProgramUniquePtr shaderProgram;
    shaderProgram.reset(new ShaderProgram);
    SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddShaderProgram()]  Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] Start adding shader to program: {}.", t_folder);

#if defined(_WIN64) && defined(_MSC_VER)

    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/Vertex.vert"));
    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/Fragment.frag"));

    if (t_loadGeometryShader)
    {
        shaderProgram->AddGeometryShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/Geometry.geom"));
    }

#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/" + t_folder + "/Vertex.vert"));
    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/" + t_folder + "/Fragment.frag"));

    if (t_loadGeometryShader)
    {
        shaderProgram->AddGeometryShader(ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/" + t_folder + "/Geometry.geom"));
    }

#else

#error Unsupported platform or unsupported compiler!

#endif

    shaderProgram->LinkAndValidateProgram();
    shaderProgram->AddAllFoundUniforms();

    m_shaderPrograms.emplace(t_folder, std::move(shaderProgram));

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] All shader was added successfully to program {}.", t_folder);
}

void sg::ogl::resource::ShaderManager::AddComputeShaderProgram(const std::string& t_folder, const std::string& t_fileName)
{
    // todo linux pfad

    ShaderProgramUniquePtr shaderProgram;
    shaderProgram.reset(new ShaderProgram);

    shaderProgram->AddComputeShader(ShaderUtil::ReadShaderFile("res/shader/" + t_folder + "/" + t_fileName + ".comp"));

    shaderProgram->LinkAndValidateProgram();
    shaderProgram->AddAllFoundUniforms();

    const auto key{ t_folder + "_" + t_fileName };
    m_computeShaderPrograms.emplace(key, std::move(shaderProgram));
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

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

sg::ogl::resource::ShaderManager::ShaderPrograms& sg::ogl::resource::ShaderManager::GetComputeShaderPrograms() noexcept
{
    return m_computeShaderPrograms;
}

const sg::ogl::resource::ShaderManager::ShaderPrograms& sg::ogl::resource::ShaderManager::GetComputeShaderPrograms() const noexcept
{
    return m_computeShaderPrograms;
}

sg::ogl::resource::ShaderManager::ShaderProgramUniquePtr& sg::ogl::resource::ShaderManager::GetComputeShaderProgram(const std::string& t_folder, const std::string& t_fileName)
{
    const auto key{ t_folder + "_" + t_fileName };

    if (m_computeShaderPrograms.count(key) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program " + key + " not exist.");
    }

    return m_computeShaderPrograms.at(key);
}
