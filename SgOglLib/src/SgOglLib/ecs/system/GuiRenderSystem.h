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
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::TransformComponent,
                component::GuiComponent>()
            };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::GuiShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                m_guiMesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *m_guiMesh);
                m_guiMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                m_guiMesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
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

    private:
        MeshSharedPtr m_guiMesh;
    };
}
