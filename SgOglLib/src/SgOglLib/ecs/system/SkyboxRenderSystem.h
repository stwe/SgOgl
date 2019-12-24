// This file is part of the SgOgl package.
// 
// Filename: SkyboxRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/SkyboxShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class SkyboxRenderSystem : public RenderSystem<resource::shaderprogram::SkyboxShaderProgram>
    {
    public:
        explicit SkyboxRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::MeshComponent,
                component::CubemapComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkyboxShaderProgram>() };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& meshComponent = view.get<component::MeshComponent>(entity);

                meshComponent.mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
                meshComponent.mesh->DrawPrimitives();
                meshComponent.mesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            OpenGl::SetDepthFunc(GL_LEQUAL);
        }

        void FinishRendering() override
        {
            // GL_LESS is the initial depth comparison function
            OpenGl::SetDepthFunc(GL_LESS);
        }

    private:

    };
}
