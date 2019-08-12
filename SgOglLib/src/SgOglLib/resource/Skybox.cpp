#include "Skybox.h"
#include "TextureManager.h"
#include "Mesh.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Skybox::Skybox(TextureManager& t_textureManager, const std::vector<std::string>& t_textureFileNames, const float t_size)
    : m_cubemapId{ t_textureManager.GetCubemapId(t_textureFileNames) }
    , m_mesh{ std::make_unique<Mesh>() }
{
    SG_OGL_CORE_ASSERT(m_mesh, "[Skybox::Skybox()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[Skybox::Skybox()] Create Skybox.");

    auto vertices{ CreateSkyboxVertices(t_size) };

    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "vPosition" },
    };

    m_mesh->Allocate(bufferLayout, &vertices, static_cast<int32_t>(vertices.size()));
}

sg::ogl::resource::Skybox::~Skybox() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Skybox::~Skybox()] Destruct Skybox.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

uint32_t sg::ogl::resource::Skybox::GetCubemapId() const
{
    return m_cubemapId;
}

sg::ogl::resource::Skybox::MeshUniquePtr& sg::ogl::resource::Skybox::GetMesh() noexcept
{
    return m_mesh;
}

const sg::ogl::resource::Skybox::MeshUniquePtr& sg::ogl::resource::Skybox::GetMesh() const noexcept
{
    return m_mesh;
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::vector<glm::vec3> sg::ogl::resource::Skybox::CreateSkyboxVertices(const float t_size)
{
    return std::vector<glm::vec3>
    {
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, t_size)
    };
}
