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
#include "resource/shaderprogram/GBufferPassShaderProgram.h"
#include "resource/shaderprogram/LightingPassShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/ModelManager.h"
#include "resource/Model.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class DeferredRenderSystem : public RenderSystem<resource::shaderprogram::GBufferPassShaderProgram, resource::shaderprogram::LightingPassShaderProgram>
    {
    public:
        using GBufferFboUniquePtr = std::unique_ptr<buffer::GBufferFbo>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit DeferredRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
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
            PrepareRendering();

            GeometryPass();
            LightingPass();

            //m_gbuffer->CopyDepthBufferToDefaultFramebuffer();

            FinishRendering();
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
                    gbufferPassShaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                    mesh->DrawPrimitives();
                    mesh->EndDraw();
                }

                if (modelComponent.showTriangles)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }

            m_gbuffer->UnbindFbo();

            resource::ShaderProgram::Unbind();
        }

        void LightingPass() const
        {
            OpenGl::ClearColorAndDepthBuffer();

            auto& lightingPassShaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::LightingPassShaderProgram>() };
            lightingPassShaderProgram.Bind();

            m_quadMesh->InitDraw();
            lightingPassShaderProgram.UpdateUniforms(*m_scene, *m_quadMesh, *m_gbuffer);
            m_quadMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
            m_quadMesh->EndDraw();

            resource::ShaderProgram::Unbind();
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

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
    };
}
