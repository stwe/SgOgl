// This file is part of the SgOgl package.
// 
// Filename: ModelRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/ModelShaderProgram.h"
#include "resource/shaderprogram/ModelNormalmapShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Model.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class ModelRenderSystem : public RenderSystem<resource::shaderprogram::ModelShaderProgram, resource::shaderprogram::ModelNormalmapShaderProgram>
    {
    public:
        explicit ModelRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent,
                component::TransformComponent>(
                    entt::exclude<component::SkydomeComponent>
                );

            auto& modelShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ModelShaderProgram>() };
            auto& normalmapShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ModelNormalmapShaderProgram>() };

            for (auto entity : view)
            {
                auto& modelComponent = view.get<component::ModelComponent>(entity);

                // render without normalmaps
                if (!modelComponent.useExistingNormalmaps)
                {
                    modelShaderProgram.Bind();

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

                    resource::ShaderProgram::Unbind();
                }
                // render with normalmaps
                else
                {
                    if (modelComponent.showTriangles)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }

                    for (auto& mesh : modelComponent.model->GetMeshes())
                    {
                        // check if there is a normalmap and bind the right shader
                        if (mesh->GetDefaultMaterial()->HasNormalMap())
                        {
                            normalmapShaderProgram.Bind();
                            normalmapShaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                        }
                        else
                        {
                            modelShaderProgram.Bind();
                            modelShaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                        }
                        mesh->InitDraw();
                        mesh->DrawPrimitives();
                        mesh->EndDraw();

                        // unbind shader
                        resource::ShaderProgram::Unbind();
                    }

                    if (modelComponent.showTriangles)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                }
            }

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
