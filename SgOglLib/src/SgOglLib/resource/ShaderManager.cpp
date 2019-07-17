#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "ShaderUtil.h"
#include "Config.h"
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

sg::ogl::resource::ShaderManager::ShaderManager(const PlatformOptions& t_platformOptions)
    : m_linuxPath{ t_platformOptions.path }
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
    if (m_shaderPrograms.count(t_folder) != 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::AddShaderProgram()] Shader program " + t_folder + " already exist.");
    }

    ShaderProgramUniquePtr shaderProgram;
    shaderProgram.reset(new ShaderProgram);
    SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddShaderProgram()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] Start adding shader to program: {}.", t_folder);

    std::string shaderPath;

#if defined(_WIN64) && defined(_MSC_VER)
    shaderPath = "res/shader/" + t_folder;
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    shaderPath = m_linuxPath + "res/shader/" + t_folder;
#else
    #error Unsupported platform or unsupported compiler!
#endif

    shaderProgram->AddVertexShader(ShaderUtil::ReadShaderFile(shaderPath + "/Vertex.vert"));
    shaderProgram->AddFragmentShader(ShaderUtil::ReadShaderFile(shaderPath + "/Fragment.frag"));

    if (t_loadGeometryShader)
    {
        shaderProgram->AddGeometryShader(ShaderUtil::ReadShaderFile(shaderPath + "/Geometry.geom"));
    }

    shaderProgram->LinkAndValidateProgram();
    shaderProgram->AddAllFoundUniforms();

    m_shaderPrograms.emplace(t_folder, std::move(shaderProgram));

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddShaderProgram()] All shader was added successfully to program {}.", t_folder);
}

void sg::ogl::resource::ShaderManager::AddComputeShaderProgram(const std::string& t_fileName)
{
    if (m_computeShaderPrograms.count(t_fileName) != 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::AddComputeShaderProgram()] Compute shader program " + t_fileName + " already exist.");
    }

    ShaderProgramUniquePtr shaderProgram;
    shaderProgram.reset(new ShaderProgram);
    SG_OGL_CORE_ASSERT(shaderProgram, "[ShaderManager::AddComputeShaderProgram()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddComputeShaderProgram()] Start adding compute shader to program: {}.", t_fileName);

    std::string shaderPath;

#if defined(_WIN64) && defined(_MSC_VER)
    shaderPath = "res/shader/compute";
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    shaderPath = m_linuxPath + "res/shader/compute";
#else
    #error Unsupported platform or unsupported compiler!
#endif

    shaderProgram->AddComputeShader(ShaderUtil::ReadShaderFile(shaderPath + "/" + t_fileName + ".comp"));
    shaderProgram->LinkAndValidateProgram();
    shaderProgram->AddAllFoundUniforms();

    m_computeShaderPrograms.emplace(t_fileName, std::move(shaderProgram));

    SG_OGL_CORE_LOG_DEBUG("[ShaderManager::AddComputeShaderProgram()] A compute shader was added successfully to program {}.", t_fileName);
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

sg::ogl::resource::ShaderManager::ShaderProgramUniquePtr& sg::ogl::resource::ShaderManager::GetComputeShaderProgram(const std::string& t_name)
{
    if (m_computeShaderPrograms.count(t_name) == 0)
    {
        throw SG_OGL_EXCEPTION("[ShaderManager::GetComputeShaderProgram()] Compute shader program " + t_name + " not exist.");
    }

    return m_computeShaderPrograms.at(t_name);
}
