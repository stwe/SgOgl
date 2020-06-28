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
        using PointLightContainer = std::vector<light::PointLight>;
        using DirectionalLightContainer = std::vector<light::DirectionalLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit InstancingRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            debugName = "InstancingRenderer";
        }

        InstancingRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            debugName = "InstancingRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            PointLightContainer pointLights;
            m_scene->GetApplicationContext()->registry.view<light::PointLight>().each([&pointLights](auto, auto& t_pointLight)
            {
                pointLights.push_back(t_pointLight);
            });

            DirectionalLightContainer directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::InstancingShaderProgram>() };
            shaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<component::ModelInstancesComponent>() };

            for (auto entity : view)
            {
                auto& modelInstancesComponent{ view.get<component::ModelInstancesComponent>(entity) };

                if (modelInstancesComponent.showTriangles)
                {
                    OpenGl::EnableWireframeMode();
                }

                for (auto& mesh : modelInstancesComponent.model->GetMeshes())
                {
                    mesh->InitDraw();
                    shaderProgram.UpdateUniforms(*m_scene, entity, *mesh, pointLights, directionalLights);
                    mesh->DrawInstanced(modelInstancesComponent.instances);
                    mesh->EndDraw();
                }

                if (modelInstancesComponent.showTriangles)
                {
                    OpenGl::DisableWireframeMode();
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
