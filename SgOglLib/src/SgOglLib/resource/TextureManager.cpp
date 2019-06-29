#include "TextureManager.h"
#include "OpenGl.h"
#include "Log.h"
#include "SgOglException.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::TextureManager::~TextureManager() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[TextureManager::~TextureManager()] Execute the TextureManager destructor.");
    CleanUp();
}

//-------------------------------------------------
// Load && Create
//-------------------------------------------------

uint32_t sg::ogl::resource::TextureManager::GetTextureIdFromPath(const std::string& t_path)
{
    uint32_t textureId;

    if (m_textures.count(t_path) == 0)
    {
        textureId = GenerateNewTextureHandle();
        LoadTextureFromFile(t_path, textureId);
        SG_OGL_CORE_LOG_DEBUG("[TextureManager::GetTextureIdFromPath()] A new texture file {} was successfully loaded and used for the new created texture handle. Id: {}", t_path, textureId);
        m_textures.emplace(t_path, textureId);
    }
    else
    {
        textureId = m_textures.at(t_path);
    }

    SG_OGL_CORE_ASSERT(textureId, "[TextureManager::GetTextureIdFromPath()] Invalid texture Id.")
    return textureId;
}

uint32_t sg::ogl::resource::TextureManager::GetTextureId(const std::string& t_name)
{
    uint32_t textureId;

    if (m_textures.count(t_name) == 0)
    {
        textureId = GenerateNewTextureHandle();
        SG_OGL_CORE_LOG_DEBUG("[TextureManager::GetTextureId()] A new texture handle for an empty texture was created. Id: {}", textureId);
        m_textures.emplace(t_name, textureId);
    }
    else
    {
        textureId = m_textures.at(t_name);
    }

    SG_OGL_CORE_ASSERT(textureId, "[TextureManager::GetTextureId()] Invalid texture Id.")
    return textureId;
}

uint32_t sg::ogl::resource::TextureManager::GetCubemapId(const std::vector<std::string>& t_pathNames)
{
    uint32_t cubemapId;

    if (m_cubemaps.count(t_pathNames) == 0)
    {
        cubemapId = GenerateNewTextureHandle();
        LoadTextureFromFile(t_pathNames, cubemapId);
        SG_OGL_CORE_LOG_DEBUG("[TextureManager::GetCubemapId()] Cubemap files successfully loaded and used for the new created texture handle. Id: {}", cubemapId);
        m_cubemaps.emplace(t_pathNames, cubemapId);
    }
    else
    {
        cubemapId = m_cubemaps.at(t_pathNames);
    }

    SG_OGL_CORE_ASSERT(cubemapId, "[TextureManager::GetCubemapId()] Invalid cubemap Id.")
    return cubemapId;
}

//-------------------------------------------------
// Filter
//-------------------------------------------------

void sg::ogl::resource::TextureManager::UseNoFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void sg::ogl::resource::TextureManager::UseNoFilter()
{
    UseNoFilter(GL_TEXTURE_2D);
}

void sg::ogl::resource::TextureManager::UseBilinearFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void sg::ogl::resource::TextureManager::UseBilinearFilter()
{
    UseBilinearFilter(GL_TEXTURE_2D);
}

void sg::ogl::resource::TextureManager::UseBilinearMipmapFilter(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void sg::ogl::resource::TextureManager::UseBilinearMipmapFilter()
{
    UseBilinearMipmapFilter(GL_TEXTURE_2D);
}

//-------------------------------------------------
// Wrapping
//-------------------------------------------------

void sg::ogl::resource::TextureManager::UseRepeatWrapping(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void sg::ogl::resource::TextureManager::UseRepeatWrapping()
{
    UseRepeatWrapping(GL_TEXTURE_2D);
}

void sg::ogl::resource::TextureManager::UseClampToEdgeWrapping(const uint32_t t_target)
{
    glTexParameteri(t_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(t_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void sg::ogl::resource::TextureManager::UseClampToEdgeWrapping()
{
    UseClampToEdgeWrapping(GL_TEXTURE_2D);
}

//-------------------------------------------------
// Bind / Unbind
//-------------------------------------------------

void sg::ogl::resource::TextureManager::Bind(const uint32_t t_textureId, const uint32_t t_target)
{
    SG_OGL_CORE_ASSERT(t_textureId, "[TextureManager::Bind()] Invalid texture Id.")
    glBindTexture(t_target, t_textureId);
}

void sg::ogl::resource::TextureManager::Bind(const uint32_t t_textureId)
{
    Bind(t_textureId, GL_TEXTURE_2D);
}

void sg::ogl::resource::TextureManager::Unbind(const uint32_t t_target)
{
    glBindTexture(t_target, 0);
}

void sg::ogl::resource::TextureManager::Unbind()
{
    Unbind(GL_TEXTURE_2D);
}

void sg::ogl::resource::TextureManager::BindForReading(const uint32_t t_textureId, const uint32_t t_textureUnit, const uint32_t t_target)
{
    SG_OGL_CORE_ASSERT(t_textureId, "[TextureManager::BindForReading()] Invalid texture Id.")
    glActiveTexture(t_textureUnit);

    Bind(t_textureId, t_target);
}

void sg::ogl::resource::TextureManager::BindForReading(const uint32_t t_textureId, const uint32_t t_textureUnit)
{
    BindForReading(t_textureId, t_textureUnit, GL_TEXTURE_2D);
}

//-------------------------------------------------
// Get metadata
//-------------------------------------------------

const sg::ogl::resource::TextureManager::Meta& sg::ogl::resource::TextureManager::GetMetadata(const std::string& t_path) const
{
    return m_metadata.at(t_path);
}

//-------------------------------------------------
// Image loader
//-------------------------------------------------

uint32_t sg::ogl::resource::TextureManager::GenerateNewTextureHandle()
{
    uint32_t textureId{ 0 };

    glGenTextures(1, &textureId);

    return textureId;
}

void sg::ogl::resource::TextureManager::LoadTextureFromFile(const std::string& t_path, const uint32_t t_textureId)
{
    SG_OGL_CORE_ASSERT(t_textureId, "[TextureManager::LoadTextureFromFile()] Invalid texture Id.")

    int nrChannels, width, height;
    const auto image{ stbi_load(t_path.c_str(), &width, &height, &nrChannels, 0) };
    if (image)
    {
        uint32_t format{ 0 };
        if (nrChannels == STBI_grey)
            format = GL_RED;
        else if (nrChannels == STBI_rgb)
            format = GL_RGB;
        else if (nrChannels == STBI_rgb_alpha)
            format = GL_RGBA;

        SG_OGL_CORE_ASSERT(format, "[TextureManager::LoadTextureFromFile()] Invalid image format.")

        Meta meta;
        meta.nrChannels = nrChannels;
        meta.width = width;
        meta.height = height;
        m_metadata.emplace(t_path, meta);

        Bind(t_textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        UseBilinearMipmapFilter();
        UseRepeatWrapping();

        stbi_image_free(image);
    }
    else
    {
        throw SG_OGL_EXCEPTION("[TextureManager::LoadTextureFromFile()] Texture failed to load at path: " + t_path);
    }
}

void sg::ogl::resource::TextureManager::LoadTextureFromFile(const std::vector<std::string>& t_pathNames, const uint32_t t_textureId)
{
    const auto count{ t_pathNames.size() };

    SG_OGL_CORE_ASSERT(count == 6, "[TextureManager::LoadTextureFromFile()] Invalid number of images.")
    SG_OGL_CORE_ASSERT(t_textureId, "[TextureManager::LoadTextureFromFile()] Invalid texture Id.")

    Bind(t_textureId, GL_TEXTURE_CUBE_MAP);

    int nrChannels, width, height;
    for (auto i{ 0u }; i < count; ++i)
    {
        const auto& fileName{ t_pathNames[i].c_str() };
        const auto image{ stbi_load(fileName, &width, &height, &nrChannels, 0) };
        if (image)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            stbi_image_free(image);
        }
        else
        {
            throw SG_OGL_EXCEPTION("[TextureManager::LoadTextureFromFile()] Cubemap texture failed to load at path: " + t_pathNames[i]);
        }
    }

    UseBilinearFilter(GL_TEXTURE_CUBE_MAP);
    UseClampToEdgeWrapping(GL_TEXTURE_CUBE_MAP);
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::resource::TextureManager::CleanUp()
{
    SG_OGL_CORE_LOG_DEBUG("[TextureManager::CleanUp()] Start the OpenGL clean up process for textures.");

    for (const auto& texture : m_textures)
    {
        glDeleteTextures(1, &texture.second);
        SG_OGL_CORE_LOG_DEBUG("[TextureManager::CleanUp()] Texture was deleted. Id: {}", texture.second);
    }

    for (const auto& cubemap : m_cubemaps)
    {
        glDeleteTextures(1, &cubemap.second);
        SG_OGL_CORE_LOG_DEBUG("[TextureManager::CleanUp()] Cubemap was deleted. Id: {}", cubemap.second);
    }
}
