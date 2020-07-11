// This file is part of the SgOgl package.
// 
// Filename: SunRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/SunShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "light/Sun.h"

namespace sg::ogl::ecs::system
{
    class SunRenderSystem : public RenderSystem<resource::shaderprogram::SunShaderProgram>
    {
    public:
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit SunRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_sunQuadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::SUN_QUAD_MESH);
            name = "SunRenderer";
        }

        SunRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_sunQuadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::SUN_QUAD_MESH);
            name = "SunRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<light::Sun>() };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SunShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                m_sunQuadMesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *m_sunQuadMesh);
                m_sunQuadMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                m_sunQuadMesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::DisableDepthTesting();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
            OpenGl::EnableDepthTesting();
        }

    protected:

    private:
        MeshSharedPtr m_sunQuadMesh;
    };
}
