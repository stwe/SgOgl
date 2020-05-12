// This file is part of the SgOgl package.
// 
// Filename: Config.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <lua.hpp>
#include "Config.h"
#include "Core.h"
#include "SgOglException.h"

void sg::ogl::Config::LoadOptions(
    const std::string& t_fileName,
    std::string& t_libResFolder,
    WindowOptions& t_windowOptions,
    ProjectionOptions& t_projectionOptions
)
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Config::LoadOptions()] Loading options from {}.", t_fileName);

    auto* luaState{ luaL_newstate() };

    const auto result{ luaL_dofile(luaState, t_fileName.c_str()) };
    if (result != LUA_OK)
    {
        PrintLuaErrorMessage(luaState);
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] Error while loading Lua-Config file " + t_fileName + ".");
    }

    lua_getglobal(luaState, "libResFolder");
    if (lua_isstring(luaState, -1))
    {
        t_libResFolder = lua_tostring(luaState, -1);
    }

    lua_getglobal(luaState, "window");
    if (lua_istable(luaState, -1))
    {
        lua_pushstring(luaState, "title");
        lua_gettable(luaState, -2);
        t_windowOptions.title = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "compatibleProfile");
        lua_gettable(luaState, -2);
        t_windowOptions.compatibleProfile = lua_toboolean(luaState, -1);
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "debugContext");
        lua_gettable(luaState, -2);
        t_windowOptions.debugContext = lua_toboolean(luaState, -1);
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "antialiasing");
        lua_gettable(luaState, -2);
        t_windowOptions.antialiasing = lua_toboolean(luaState, -1);
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "printFrameRate");
        lua_gettable(luaState, -2);
        t_windowOptions.printFrameRate = lua_toboolean(luaState, -1);
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "glMajor");
        lua_gettable(luaState, -2);
        t_windowOptions.glMajor = static_cast<int>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "glMinor");
        lua_gettable(luaState, -2);
        t_windowOptions.glMinor = static_cast<int>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "fps");
        lua_gettable(luaState, -2);
        t_windowOptions.fps = static_cast<float>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);
    }

    lua_getglobal(luaState, "projection");
    if (lua_istable(luaState, -1))
    {
        lua_pushstring(luaState, "fovDeg");
        lua_gettable(luaState, -2);
        t_projectionOptions.fovDeg = static_cast<float>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "width");
        lua_gettable(luaState, -2);
        t_projectionOptions.width = static_cast<int>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "height");
        lua_gettable(luaState, -2);
        t_projectionOptions.height = static_cast<int>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "near");
        lua_gettable(luaState, -2);
        t_projectionOptions.nearPlane = static_cast<float>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);

        lua_pushstring(luaState, "far");
        lua_gettable(luaState, -2);
        t_projectionOptions.farPlane = static_cast<float>(lua_tonumber(luaState, -1));
        lua_pop(luaState, 1);
    }

    lua_close(luaState);
}

void sg::ogl::Config::PrintLuaErrorMessage(lua_State* t_luaState)
{
    // The error message is on top of the stack.
    // Fetch it, print it and then pop it off the stack.
    Log::SG_OGL_CORE_LOG_FATAL("[Config::PrintLuaErrorMessage()] Lua error {}.", lua_tostring(t_luaState, -1));
    lua_pop(t_luaState, 1);
}
