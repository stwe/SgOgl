// This file is part of the SgOgl package.
// 
// Filename: ParticleRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "ecs/component/Components.h"
#include "resource/shaderprogram/ParticleShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"

namespace sg::ogl::ecs::system
{
    class ParticleRenderSystem : public RenderSystem<resource::shaderprogram::ParticleShaderProgram>
    {
    public:
        explicit ParticleRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(const double t_dt) override
        {
            auto view = m_scene->GetApplicationContext()->registry.view<
                component::ParticleEmitterComponent,
                component::MeshComponent>();

            for (auto entity : view)
            {
                auto& emitterComponent = view.get<component::ParticleEmitterComponent>(entity);

                emitterComponent.particleEmitter->Update(t_dt);
            }
        }

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::ParticleEmitterComponent,
                component::MeshComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ParticleShaderProgram>() };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& meshComponent = view.get<component::MeshComponent>(entity);
                auto& emitterComponent = view.get<component::ParticleEmitterComponent>(entity);

                meshComponent.mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
                emitterComponent.particleEmitter->Render();
                glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<int32_t>(emitterComponent.particleEmitter->GetParticles().size()));
                //meshComponent.mesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                meshComponent.mesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::DisableWritingIntoDepthBuffer();
        }

        void FinishRendering() override
        {
            OpenGl::EnableWritingIntoDepthBuffer();
            OpenGl::DisableBlending();
        }

    private:

    };
}
