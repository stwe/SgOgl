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
#include "math/Transform.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/Sun.h"

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
        {
            debugName = "ForwardRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            std::vector<light::PointLight> pointLights;
            m_scene->GetApplicationContext()->registry.view<light::PointLight>().each([&pointLights](auto t_entity, auto& t_pointLight)
            {
                pointLights.push_back(t_pointLight);
            });

            std::vector<light::DirectionalLight> directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto t_entity, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto t_entity, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& modelShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ModelShaderProgram>() };
            modelShaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent, math::Transform>()
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
                    // todo: set fakeNormals
                    mesh->InitDraw();
                    modelShaderProgram.UpdateUniforms(*m_scene, entity, *mesh, pointLights, directionalLights);
                    mesh->DrawPrimitives();
                    mesh->EndDraw();
                }

                if (modelComponent.showTriangles)
                {
                    OpenGl::DisableWireframeMode();
                }
            }

            resource::ShaderProgram::Unbind();
        }

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

    protected:

    private:

    };
}
