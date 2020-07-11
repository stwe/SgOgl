// This file is part of the SgOgl package.
// 
// Filename: GuiRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/GuiShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "ecs/component/Components.h"
#include "math/Transform.h"

namespace sg::ogl::ecs::system
{
    class GuiRenderSystem : public RenderSystem<resource::shaderprogram::GuiShaderProgram>
    {
    public:
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit GuiRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_guiMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::GUI_MESH);
            name = "GuiRenderer";
        }

        GuiRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_guiMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::GUI_MESH);
            name = "GuiRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::GuiShaderProgram>() };
            shaderProgram.Bind();

            m_scene->GetApplicationContext()->registry.view<math::Transform, component::GuiComponent>().each(
                [&](auto t_entity, auto&, auto&)
            {
                m_guiMesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, t_entity, *m_guiMesh);
                m_guiMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                m_guiMesh->EndDraw();
            });

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::DisableDepthTesting();
        }

        void FinishRendering() override
        {
            OpenGl::EnableDepthTesting();
            OpenGl::DisableBlending();
        }

    protected:

    private:
        MeshSharedPtr m_guiMesh;
    };
}
