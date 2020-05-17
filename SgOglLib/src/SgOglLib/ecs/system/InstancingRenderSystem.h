// This file is part of the SgOgl package.
// 
// Filename: InstancingRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "ecs/component/Components.h"
#include "resource/shaderprogram/InstancingShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Model.h"

namespace sg::ogl::ecs::system
{
    class InstancingRenderSystem : public RenderSystem<resource::shaderprogram::InstancingShaderProgram>
    {
    public:
        explicit InstancingRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            auto view = m_scene->GetApplicationContext()->registry.view<
                component::InstancesComponent,
                component::ModelComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::InstancingShaderProgram>() };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& instancesComponent = view.get<component::InstancesComponent>(entity);
                auto& modelComponent = view.get<component::ModelComponent>(entity);

                for (auto& mesh : modelComponent.model->GetMeshes())
                {
                    mesh->InitDraw();
                    shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                    mesh->DrawInstanced(instancesComponent.instances);
                    mesh->EndDraw();
                }
            }

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
        }

    protected:

    private:

    };
}
