#pragma once

#include <vector>
#include <string>

struct lua_State;

namespace sg::ogl::lua
{
    class LuaHelper
    {
    public:
        static void LoadScript(lua_State* t_luaState, const std::string& t_filename);
        static void LuaGetToStack(lua_State* t_luaState, const std::string& t_variableName);
        static void LoadGetKeysFunction(lua_State* t_luaState);
        static std::vector<std::string> GetTableKeys(lua_State* t_luaState, const std::string& t_name);

        static void StackDump(lua_State* t_luaState);
    protected:

    private:

    };
}
