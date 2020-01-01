// This file is part of the SgOgl package.
// 
// Filename: ForwardRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/ModelShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Model.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class ForwardRenderSystem : public RenderSystem<resource::shaderprogram::ModelShaderProgram>
    {
    public:
        explicit ForwardRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto& modelShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ModelShaderProgram>() };
            modelShaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent,
                component::TransformComponent>(
                    entt::exclude<component::SkydomeComponent>
                )
            };

            for (auto entity : view)
            {
                auto& modelComponent{ view.get<component::ModelComponent>(entity) };

                if (modelComponent.showTriangles)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }

                for (auto& mesh : modelComponent.model->GetMeshes())
                {
                    mesh->InitDraw();
                    modelShaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                    mesh->DrawPrimitives();
                    mesh->EndDraw();
                }

                if (modelComponent.showTriangles)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
            OpenGl::DisableFaceCulling();
        }

    private:

    };
}
