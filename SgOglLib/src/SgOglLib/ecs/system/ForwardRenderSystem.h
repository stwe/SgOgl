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
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit ForwardRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override
        {
            AddEntityPointLights();
        }

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
                    OpenGl::EnableWireframeMode();
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
                    OpenGl::DisableWireframeMode();
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
        //-------------------------------------------------
        // Point lights
        //-------------------------------------------------

        /**
         * @brief A Point Light is added to the Scene if it does not already exist there.
         */
        void AddEntityPointLights() const
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent,
                component::TransformComponent,
                component::PointLightComponent>()
            };

            for (auto entity : view)
            {
                auto& pointLightComponent{ view.get<component::PointLightComponent>(entity) };
                m_scene->AddEntityPointLight(pointLightComponent.name, pointLightComponent.pointLight);

                // todo: remove
            }
        }
    };
}
