// This file is part of the SgOgl package.
// 
// Filename: RenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Core.h"
#include "RenderSystemInterface.h"
#include "scene/Scene.h"
#include "resource/ShaderManager.h"

namespace sg::ogl::ecs::system
{
    template <typename ...TShaderProgram>
    class RenderSystem : public RenderSystemInterface
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderSystem() = delete;

        explicit RenderSystem(scene::Scene* t_scene)
            : m_scene{ t_scene }
        {
            SG_OGL_CORE_ASSERT(m_scene, "[RenderSystem::RenderSystem()] Null pointer.");

            LoadShader();
        }

        RenderSystem(const int t_priority, scene::Scene* t_scene)
            : m_scene{ t_scene }
        {
            SG_OGL_CORE_ASSERT(m_scene, "[RenderSystem::RenderSystem()] Null pointer.");

            priority = t_priority;

            LoadShader();
        }

        RenderSystem(const RenderSystem& t_other) = delete;
        RenderSystem(RenderSystem&& t_other) noexcept = delete;
        RenderSystem& operator=(const RenderSystem& t_other) = delete;
        RenderSystem& operator=(RenderSystem&& t_other) noexcept = delete;

        virtual ~RenderSystem() noexcept = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(double t_dt) override {}
        void Render() override {};
        void PrepareRendering() override {}
        void FinishRendering() override {}

    protected:
        scene::Scene* m_scene{ nullptr };

    private:
        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void LoadShader() const
        {
            (m_scene->GetApplicationContext()->GetShaderManager().AddShaderProgram<TShaderProgram>(), ...);
        }
    };
}
