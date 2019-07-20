#pragma once

#include <string>
#include <map>
#include "resource/ComputeShaderTexture.h"

namespace sg::ogl
{
    //-------------------------------------------------
    // Window options
    //-------------------------------------------------

    struct WindowOptions
    {
        std::string title{ "" };
        bool showTriangles{ false };
        bool compatibleProfile{ false };
        bool debugContext{ false };
        bool antialiasing{ false };
        bool faceCulling{ true };
        bool frustumCulling{ false };
        bool printFrameRate{ false };
        int glMajor{ 3 };
        int glMinor{ 3 };
    };

    //-------------------------------------------------
    // Projection options
    //-------------------------------------------------

    struct ProjectionOptions
    {
        float fovDeg{ 0.0f };
        int width{ 0 };
        int height{ 0 };
        float nearPlane{ 0.0f };
        float farPlane{ 0.0f };
    };

    //-------------------------------------------------
    // Platform options
    //-------------------------------------------------

    struct PlatformOptions
    {
        std::string path{"" };
    };

    //-------------------------------------------------
    // Terrain options
    //-------------------------------------------------

    struct TerrainOptions
    {
        using TextureName = std::string;
        using TexturePath = std::string;
        using TexturePack = std::map<TextureName, TexturePath>;

        float xPos{ 0.0f };
        float zPos{ 0.0f };
        float scaleXz{ 1.0f };
        float scaleY{ 1.0f };
        std::string heightmapPath;
        TexturePack texturePack;
        resource::ComputeShaderTexture normalmap;
        resource::ComputeShaderTexture splatmap;
        float normalStrength{ 60.0f };
    };

    //-------------------------------------------------
    // Config
    //-------------------------------------------------

    class Config
    {
    public:
        static bool ToBool(const std::string& t_value);

        static void LoadOptions(
            const std::string& t_fileName,
            WindowOptions& t_windowOptions,
            ProjectionOptions& t_projectionOptions,
            PlatformOptions& t_platformOptions
        );

        static void LoadOptions(const std::string& t_fileName, TerrainOptions& t_terrainOptions);

    protected:

    private:

    };
}
