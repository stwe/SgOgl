#include <lua.hpp>
#include "LuaHelper.h"
#include "SgOglException.h"
#include "Log.h"

void sg::ogl::lua::LuaHelper::LoadScript(lua_State* t_luaState, const std::string& t_filename)
{
    // luaL_loadfile load the chunk from the file and then calls lua_pcall to run the
    // compiled chunk; in case of errors, these functions push the error message onto
    // the stack and return a non-zero error code

    if (luaL_loadfile(t_luaState, t_filename.c_str()) || lua_pcall(t_luaState, 0, 0, 0))
    {
        const auto* error{ lua_tostring(t_luaState, -1) };
        lua_pop(t_luaState, 1);
        throw SG_OGL_EXCEPTION("[LuaHelper::LoadScript()] Failed to load " + t_filename + ". Error: " + error);
    }
}

void sg::ogl::lua::LuaHelper::LuaGetToStack(lua_State* t_luaState, const std::string& t_variableName)
{
    auto level{ 0 };
    std::string var;

    for (auto i{ 0u }; i < t_variableName.size(); ++i)
    {
        if (t_variableName.at(i) == '.')
        {
            if (level == 0)
            {
                lua_getglobal(t_luaState, var.c_str());
            }
            else
            {
                lua_getfield(t_luaState, -1, var.c_str());
            }

            if (lua_isnil(t_luaState, -1))
            {
                //std::cout << "Error, can't get " << t_variableName << std::endl;
                return;
            }

            var = "";
            ++level;
        }
        else
        {
            var += t_variableName.at(i);
        }
    }
    if (level == 0)
    {
        lua_getglobal(t_luaState, var.c_str());
    }
    else
    {
        lua_getfield(t_luaState, -1, var.c_str());
    }

    // no need to remove anything
    if (level == 0)
    {
        return;
    }

    const auto tableIndex{ lua_gettop(t_luaState) - level };

    lua_replace(t_luaState, tableIndex);
    lua_settop(t_luaState, tableIndex);
}

void sg::ogl::lua::LuaHelper::LoadGetKeysFunction(lua_State* t_luaState)
{
    const std::string code =
        R"(function getKeys(t)
            s = {}
            for k, v in pairs(t) do
                table.insert(s, k)
            end 
            return s 
        end)";

    luaL_dostring(t_luaState, code.c_str());
}

std::vector<std::string> sg::ogl::lua::LuaHelper::GetTableKeys(lua_State* t_luaState, const std::string& t_name)
{
    // 1. push the function to be called
    // 2. push the arguments to the call
    // 3. use lua_pcall
    // 4. pop the results from the stack

    lua_getglobal(t_luaState, "getKeys");
    if (lua_isnil(t_luaState, -1))
    {
        lua_pop(t_luaState, -1); // todo: to remove nil from the stack
        LoadGetKeysFunction(t_luaState);
        lua_getglobal(t_luaState, "getKeys");
    }

    LuaGetToStack(t_luaState, t_name);

    /*

    |---------------------|
    |                     |
    |---------------------|
    | name: table         |
    |-------------------- |
    | getKeys(): function |
    |---------------------|

    */

    lua_pcall(t_luaState, 1, 1, 0);

    lua_pushnil(t_luaState);

    /*

    |---------------------|
    |                     |
    |---------------------|
    | nil                 |
    |-------------------- |
    | s: table            |
    |---------------------|

    */

    std::vector<std::string> keys;

    while (lua_next(t_luaState, -2))
    {
        if (lua_type(t_luaState, -1) == LUA_TSTRING)
        {
            keys.emplace_back(lua_tostring(t_luaState, -1));
        }

        lua_pop(t_luaState, 1);
    }

    lua_settop(t_luaState, 0); // remove s table from stack

    return keys;
}

void sg::ogl::lua::LuaHelper::StackDump(lua_State* t_luaState)
{
    const auto top{ lua_gettop(t_luaState) };

    for (auto i{ 1 }; i <= top; ++i)
    {
        const auto t{ lua_type(t_luaState, i) };
        switch (t)
        {
        case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(t_luaState, i));
            break;

        case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(t_luaState, i) ? "true" : "false");
            break;

        case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(t_luaState, i));
            break;

        default:  /* other values */
            printf("%s", lua_typename(t_luaState, t));
            break;
        }

        printf("  ");
    }

    printf("\n");
}
