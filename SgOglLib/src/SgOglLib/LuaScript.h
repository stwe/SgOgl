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

#include "Log.h"
#include "scene/Scene.h"

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

        template <typename T>
        void CreateRendererUsertype(const std::string& t_name)
        {
            Log::SG_OGL_CORE_LOG_DEBUG("[LuaScript::CreateRendererUsertype()] Add {} to the current Scene.", t_name);

            m_lua.new_usertype<T>(
                t_name,
                sol::constructors<
                    T(scene::Scene*),
                    T(int t_priority, scene::Scene*)
                >(),
                "new", sol::factories(
                    [&](scene::Scene* t_currentScene)
                    {
                        t_currentScene->renderer.push_back(std::make_unique<T>(t_currentScene));
                        return t_currentScene->renderer.back().get();
                    },
                    [&](int t_priority, scene::Scene* t_currentScene)
                    {
                        t_currentScene->renderer.push_back(std::make_unique<T>(t_priority, t_currentScene));
                        return t_currentScene->renderer.back().get();
                    }
                )
            );
        }

        void CreateCameraUsertypes();
        void CreateResourceUsertypes();
        void CreateComponentUsertypes();
        void CreateEcsRegistryUsertype();
    };
}
