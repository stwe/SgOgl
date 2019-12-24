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
        explicit GuiRenderSystem(sg::ogl::scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::TransformComponent,
                component::MeshComponent,
                component::GuiComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::GuiShaderProgram>() };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& meshComponent = view.get<component::MeshComponent>(entity);

                meshComponent.mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
                meshComponent.mesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                meshComponent.mesh->EndDraw();
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

    };
}
