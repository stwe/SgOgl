// This file is part of the SgOgl package.
// 
// Filename: DeferredRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "buffer/GBufferFbo.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/Sun.h"
#include "resource/shaderprogram/GBufferPassShaderProgram.h"
#include "resource/shaderprogram/LightingPassShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/ModelManager.h"
#include "resource/Model.h"
#include "ecs/component/Components.h"
#include "math/Transform.h"

namespace sg::ogl::ecs::system
{
    class DeferredRenderSystem : public RenderSystem<
        resource::shaderprogram::GBufferPassShaderProgram,
        resource::shaderprogram::LightingPassShaderProgram>
    {
    public:
        using GBufferFboUniquePtr = std::unique_ptr<buffer::GBufferFbo>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        using PointLightContainer = std::vector<light::PointLight>;
        using DirectionalLightContainer = std::vector<light::DirectionalLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit DeferredRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            name = "DeferredRenderer";

            m_gbuffer = std::make_unique<buffer::GBufferFbo>(m_scene->GetApplicationContext());
            m_quadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::QUAD_MESH);
        }

        DeferredRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            name = "DeferredRenderer";

            m_gbuffer = std::make_unique<buffer::GBufferFbo>(m_scene->GetApplicationContext());
            m_quadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::QUAD_MESH);
        }

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const buffer::GBufferFbo& GetFbo() const noexcept { return *m_gbuffer; }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            GeometryPass();
            LightingPass();

            //m_gbuffer->CopyDepthBufferToDefaultFramebuffer();
        }

        void PrepareRendering() override
        {
            OpenGl::DisableBlending();   // disable GL_BLEND
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
            OpenGl::DisableFaceCulling();
        }

    protected:

    private:
        GBufferFboUniquePtr m_gbuffer;
        MeshSharedPtr m_quadMesh;

        //-------------------------------------------------
        // Passes
        //-------------------------------------------------

        void GeometryPass() const
        {
            m_gbuffer->BindFbo();

            OpenGl::ClearColorAndDepthBuffer();

            auto& gbufferPassShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::GBufferPassShaderProgram>() };
            gbufferPassShaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::ModelComponent,
                math::Transform>(
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
                    gbufferPassShaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                    mesh->DrawPrimitives();
                    mesh->EndDraw();
                }

                if (modelComponent.showTriangles)
                {
                    OpenGl::DisableWireframeMode();
                }
            }

            m_gbuffer->UnbindFbo();

            resource::ShaderProgram::Unbind();
        }

        void LightingPass() const
        {
            OpenGl::ClearColorAndDepthBuffer();

            PointLightContainer pointLights;
            m_scene->GetApplicationContext()->registry.view<light::PointLight>().each([&pointLights](auto t_entity, auto& t_pointLight)
            {
                pointLights.push_back(t_pointLight);
            });

            DirectionalLightContainer directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto t_entity, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto t_entity, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& lightingPassShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::LightingPassShaderProgram>() };
            lightingPassShaderProgram.Bind();

            m_quadMesh->InitDraw();
            lightingPassShaderProgram.UpdateUniforms(*m_scene, *m_gbuffer, pointLights, directionalLights);
            m_quadMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
            m_quadMesh->EndDraw();

            resource::ShaderProgram::Unbind();
        }
    };
}
