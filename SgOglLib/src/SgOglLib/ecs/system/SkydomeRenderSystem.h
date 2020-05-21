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
#include "ecs/component/Components.h"
#include "math/Transform.h"

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
        {
            debugName = "SkydomeRenderer";
        }

        SkydomeRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            debugName = "SkydomeRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::DomeShaderProgram>() };
            shaderProgram.Bind();

            m_scene->GetApplicationContext()->registry.view<component::ModelComponent, math::Transform, component::SkydomeComponent>().each(
            [&](auto t_entity, auto& t_modelComponent, auto&)
            {
                t_modelComponent.model->GetMeshes()[0]->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, t_entity, *t_modelComponent.model->GetMeshes()[0]);
                t_modelComponent.model->GetMeshes()[0]->DrawPrimitives();
                t_modelComponent.model->GetMeshes()[0]->EndDraw();
            });

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            //OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            //OpenGl::DisableFaceCulling();
        }

    protected:

    private:

    };
}
