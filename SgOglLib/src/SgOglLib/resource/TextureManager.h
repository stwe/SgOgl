#pragma once

#include <map>
#include <vector>
#include <string>
#include "Core.h"

namespace sg::ogl::resource
{
    class SG_OGL_API TextureManager
    {
    public:
        using Textures = std::map<std::string, uint32_t>;
        using Cubemaps = std::map<std::vector<std::string>, uint32_t>;

        //-------------------------------------------------
        // Meta
        //-------------------------------------------------

        struct Meta
        {
            int nrChannels{ -1 };
            int width{ 0 }; 
            int height{ 0 };
        };

        using Metadata = std::map<std::string, Meta>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TextureManager() = default;

        TextureManager(const TextureManager& t_other) = delete;
        TextureManager(TextureManager&& t_other) noexcept = delete;
        TextureManager& operator=(const TextureManager& t_other) = delete;
        TextureManager& operator=(TextureManager&& t_other) noexcept = delete;

        ~TextureManager() noexcept = default;

        //-------------------------------------------------
        // Load && Create
        //-------------------------------------------------

        /**
         * @brief Get the texture handle for the given file path.
         * @param t_path The file path of the texture.
         * @return The texture handle.
         */
        uint32_t GetTextureIdFromPath(const std::string& t_path);

        /**
         * @brief Get the texture handle for the given name.
         * @param t_name The name of the texture.
         * @return The texture handle.
         */
        uint32_t GetTextureId(const std::string& t_name);

        /**
         * @brief Get the cubemap handle for the given file names.
         * @param t_pathNames The file names of the cubemap.
         * @return The cubemap handle.
         */
        uint32_t GetCubemapId(const std::vector<std::string>& t_pathNames);

        //-------------------------------------------------
        // Filter
        //-------------------------------------------------

        /**
         * @brief Use the default texture filtering method (nearest neighbor filtering) of OpenGL.
         *        OpenGL selects the pixel which center is closest to the texture coordinate.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseNoFilter()
         */
        static void UseNoFilter(uint32_t t_target);
        static void UseNoFilter();

        /**
         * @brief Takes an interpolated value from the texture coordinate's neighboring texels.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseBilinearFilter()
         */
        static void UseBilinearFilter(uint32_t t_target);
        static void UseBilinearFilter();

        /**
         * @brief Linearly interpolates between the two closest mipmaps and samples the texture via linear interpolation.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseBilinearMipmapFilter()
         */
        static void UseBilinearMipmapFilter(uint32_t t_target);
        static void UseBilinearMipmapFilter();

        //-------------------------------------------------
        // Wrapping
        //-------------------------------------------------

        /**
         * @brief The default behavior for textures. Repeats the texture image.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseRepeatWrapping()
         */
        static void UseRepeatWrapping(uint32_t t_target);
        static void UseRepeatWrapping();

        /**
         * @brief Clamps the coordinates between 0 and 1.
         * @param t_target Specifies the target to which the texture is bound for glTexParameter functions.
         * @see UseClampToEdgeWrapping()
         */
        static void UseClampToEdgeWrapping(uint32_t t_target);
        static void UseClampToEdgeWrapping();

        //-------------------------------------------------
        // Bind / Unbind
        //-------------------------------------------------

        static void Bind(uint32_t t_textureId, uint32_t t_target);
        static void Bind(uint32_t t_textureId);

        static void Unbind(uint32_t t_target);
        static void Unbind();

        static void BindForReading(uint32_t t_textureId, uint32_t t_textureUnit, uint32_t t_target);
        static void BindForReading(uint32_t t_textureId, uint32_t t_textureUnit);

        //-------------------------------------------------
        // Get metadata
        //-------------------------------------------------

        const Meta& GetMetadata(const std::string& t_path) const;

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp();

    protected:

    private:
        Textures m_textures;
        Cubemaps m_cubemaps;
        Metadata m_metadata;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static uint32_t GenerateNewTextureHandle();

        //-------------------------------------------------
        // Image loader
        //-------------------------------------------------

        void LoadTextureFromFile(const std::string& t_path, uint32_t t_textureId);

        static void LoadTextureFromFile(const std::vector<std::string>& t_pathNames, uint32_t t_textureId);

    };
}
