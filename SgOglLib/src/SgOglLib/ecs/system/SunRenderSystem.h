// This file is part of the SgOgl package.
// 
// Filename: SunRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/SunShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class SunRenderSystem : public RenderSystem<resource::shaderprogram::SunShaderProgram>
    {
    public:
        explicit SunRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            // todo create Mesh - remove MeshComponent
        }

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view{ m_scene->GetApplicationContext()->registry.view<component::MeshComponent, component::SunComponent>() };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SunShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& meshComponent{ view.get<component::MeshComponent>(entity) };

                meshComponent.mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
                meshComponent.mesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                meshComponent.mesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
        }

    private:

    };
}
