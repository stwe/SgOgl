// This file is part of the SgOgl package.
// 
// Filename: TextureManager.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <map>
#include <vector>
#include <string>

namespace sg::ogl::resource
{
    class TextureManager
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

        TextureManager();

        TextureManager(const TextureManager& t_other) = delete;
        TextureManager(TextureManager&& t_other) noexcept = delete;
        TextureManager& operator=(const TextureManager& t_other) = delete;
        TextureManager& operator=(TextureManager&& t_other) noexcept = delete;

        ~TextureManager() noexcept;

        //-------------------------------------------------
        // Load && Create
        //-------------------------------------------------

        // todo: Revise the naming of the functions

        /**
         * @brief Get the texture handle for the given file path.
         * @param t_path The file path of the texture.
         * @return The texture handle.
         */
        [[nodiscard]] uint32_t LoadTexture(const std::string& t_path);

        /**
         * @brief Get the texture handle for the given file path.
         *        The texture is flipped vertical.
         * @param t_path The file path of the texture.
         * @return The texture handle.
         */
        [[nodiscard]] uint32_t LoadTextureVerticalFlipped(const std::string& t_path);

        /**
         * @brief Get the texture handle for the given file path.
         * @param t_path The file path of the texture.
         * @param t_flipVertically When set to true, flip the image vertically,
         *                         so the first pixel corresponds to the lower left corner of the image.
         * @return The texture handle.
         */
        [[nodiscard]] uint32_t GetTextureIdFromPath(const std::string& t_path, bool t_flipVertically = false);

        /**
         * @brief Load 16-bit grayscale heightmaps.
         * @param t_path The file path of the heightmap.
         * @return The texture handle.
         */
        [[nodiscard]] uint32_t Get16BitHeightmapIdFromPath(const std::string& t_path);

        /**
         * @brief Get the texture handle for the given name.
         * @param t_name The name of the texture.
         * @return The texture handle.
         */
        [[nodiscard]] uint32_t GetTextureId(const std::string& t_name);

        /**
         * @brief Get the cubemap handle for the given file names.
         * @param t_pathNames The file names of the cubemap.
         * @return The cubemap handle.
         */
        [[nodiscard]] uint32_t GetCubemapId(const std::vector<std::string>& t_pathNames);

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

        [[nodiscard]] const Meta& GetMetadata(const std::string& t_path) const;

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

        void LoadTextureFromFile(const std::string& t_path, uint32_t t_textureId, bool t_flipVertically);
        void Load16BitHeightmap(const std::string& t_path, uint32_t t_textureId);
        void LoadTextureFromDdsFile(const std::string& t_path, uint32_t t_textureId) const;

        static void LoadTextureFromFile(const std::vector<std::string>& t_pathNames, uint32_t t_textureId);

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp();
    };
}
