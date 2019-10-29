// This file is part of the SgOgl package.
// 
// Filename: RenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "scene/Scene.h"

namespace sg::ogl::ecs::system
{
    template <typename TShaderProgram>
    class RenderSystem
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderSystem() = delete;

        explicit RenderSystem(scene::Scene* t_scene)
            : m_scene{ t_scene }
        {
            SG_OGL_CORE_ASSERT(m_scene, "[RenderSystem::RenderSystem()] Null pointer.")

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

        std::string GetShaderFolderName() const { return m_shaderFolderName; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void Update(double t_dt) {}
        virtual void Render() = 0;

    protected:
        /**
         * @brief Pointer to the parent Scene.
         */
        scene::Scene* m_scene{ nullptr };

        /**
         * @brief Folder name of the loaded shader.
         */
        std::string m_shaderFolderName;

        virtual void PrepareRendering() {}
        virtual void FinishRendering() {}

    private:
        void LoadShader()
        {
            m_shaderFolderName = m_scene->GetApplicationContext()->GetShaderManager().AddShaderProgram<TShaderProgram>();
        }
    };
}
