// This file is part of the SgOgl package.
// 
// Filename: SkydomeRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/DomeShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class SkydomeRenderSystem : public RenderSystem<resource::shaderprogram::DomeShaderProgram>
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit SkydomeRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent,
                component::TransformComponent,
                component::SkydomeComponent>()
            };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::DomeShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& modelComponent = view.get<component::ModelComponent>(entity);

                modelComponent.model->GetMeshes()[0]->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *modelComponent.model->GetMeshes()[0]);
                modelComponent.model->GetMeshes()[0]->DrawPrimitives();
                modelComponent.model->GetMeshes()[0]->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            //OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            //OpenGl::DisableFaceCulling();
        }

    private:

    };
}
