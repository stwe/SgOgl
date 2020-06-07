// This file is part of the SgOgl package.
// 
// Filename: LuaScript.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl
{
    class LuaScript
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        LuaScript() = delete;

        LuaScript(Application* t_application, const std::string& t_fileName);

        LuaScript(const LuaScript& t_other) = delete;
        LuaScript(LuaScript&& t_other) noexcept = delete;
        LuaScript& operator=(const LuaScript& t_other) = delete;
        LuaScript& operator=(LuaScript&& t_other) noexcept = delete;

        ~LuaScript() = default;

    protected:

    private:
        Application* m_application{ nullptr };
        std::string m_fileName;

        sol::state m_lua;

        //-------------------------------------------------
        // Config
        //-------------------------------------------------

        void LoadFile();
        void InitLua();
        void RunLuaScript();
        void FinishLuaScript() const;

        //-------------------------------------------------
        // Functions / Usertypes
        //-------------------------------------------------

        void RegisterFunctions();
        void CreateGlmUsertypes();
        void CreateSceneUsertype();
        void CreateRendererUsertypes();
        void CreateCameraUsertypes();
        void CreateResourceUsertypes();
        void CreateComponentUsertypes();
        void CreateEcsRegistryUsertype();
    };
}
