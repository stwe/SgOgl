// This file is part of the SgOgl package.
// 
// Filename: ParticleRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

template <typename TShaderProgram>
class ParticleRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit ParticleRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override
    {
        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::ParticleEmitterComponent,
            sg::ogl::ecs::component::MeshComponent>();

        for (auto entity : view)
        {
            auto& emitterComponent = view.get<sg::ogl::ecs::component::ParticleEmitterComponent>(entity);

            emitterComponent.particleEmitter->Update(t_dt);
        }
    }

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::ParticleEmitterComponent,
            sg::ogl::ecs::component::MeshComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& meshComponent = view.get<sg::ogl::ecs::component::MeshComponent>(entity);
            auto& emitterComponent = view.get<sg::ogl::ecs::component::ParticleEmitterComponent>(entity);

            meshComponent.mesh->InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
            emitterComponent.particleEmitter->Render();
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<int32_t>(emitterComponent.particleEmitter->GetParticles().size()));
            //meshComponent.mesh->DrawPrimitives(GL_TRIANGLE_STRIP);
            meshComponent.mesh->EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableAlphaBlending();
        sg::ogl::OpenGl::DisableWritingIntoDepthBuffer();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::EnableWritingIntoDepthBuffer();
        sg::ogl::OpenGl::DisableBlending();
    }

private:

};
