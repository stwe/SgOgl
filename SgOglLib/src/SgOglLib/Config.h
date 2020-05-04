// This file is part of the SgOgl package.
// 
// Filename: Config.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include <map>

struct lua_State;

namespace sg::ogl
{
    //-------------------------------------------------
    // Window options
    //-------------------------------------------------

    struct WindowOptions
    {
        std::string title{ "" };
        bool compatibleProfile{ false };
        bool debugContext{ false };
        bool antialiasing{ false };
        bool printFrameRate{ false };
        int glMajor{ 4 };
        int glMinor{ 3 };
        double fps{ 60.0 };
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
    // Terrain options
    //-------------------------------------------------

    struct TerrainOptions
    {
        using TextureKeyName = std::string;
        using TexturePath = std::string;
        using TextureContainer = std::map<TextureKeyName, TexturePath>;

        float xPos{ 0.0f };
        float zPos{ 0.0f };
        float scaleXz{ 1.0f };
        float scaleY{ 1.0f };
        std::string heightmapPath;
        TextureContainer textureContainer;
        std::string normalmapShaderName;
        std::string splatmapShaderName;
        std::string normalmapTextureName;
        std::string splatmapTextureName;
        float normalStrength{ 60.0f };
    };

    //-------------------------------------------------
    // Config
    //-------------------------------------------------

    class Config
    {
    public:
        static void LoadOptions(
            const std::string& t_fileName,
            std::string& t_libResFolder,
            WindowOptions& t_windowOptions,
            ProjectionOptions& t_projectionOptions
        );

    protected:

    private:
        static void PrintLuaErrorMessage(lua_State* t_luaState);
    };
}
