// This file is part of the SgOgl package.
// 
// Filename: ParticleSystemRenderer.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "OpenGl.h"
#include "Application.h"
#include "resource/shaderprogram/ParticleSystemShaderProgram.h"
#include "resource/shaderprogram/ParticleSystemInstShaderProgram.h"
#include "resource/ShaderManager.h"

namespace sg::ogl::ecs::system
{
    class ParticleSystemRenderer : public RenderSystem<
        resource::shaderprogram::ParticleSystemShaderProgram,
        resource::shaderprogram::ParticleSystemInstShaderProgram
    >
    {
    public:
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit ParticleSystemRenderer(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_quadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::PARTICLE_QUAD_MESH);
            debugName = "ParticleSystemRenderer";
        }

        ParticleSystemRenderer(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_quadMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::PARTICLE_QUAD_MESH);
            debugName = "ParticleSystemRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override
        {
            m_scene->GetApplicationContext()->registry.view<component::ParticleSystemComponent>().each(
                [&](auto t_entity, component::ParticleSystemComponent& t_particleSystemComponent)
                {
                    t_particleSystemComponent.particleSystem->Update(t_dt);
                }
            );
        }

        void Render() override
        {
            m_scene->GetApplicationContext()->registry.view<component::ParticleSystemComponent>().each(
                [&](auto t_entity, component::ParticleSystemComponent& t_particleSystemComponent)
                {
                    if (t_particleSystemComponent.particleSystem->instancing)
                    {
                        t_particleSystemComponent.particleSystem->Render();

                        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ParticleSystemInstShaderProgram>() };
                        shaderProgram.Bind();

                        t_particleSystemComponent.particleSystem->GetMesh().InitDraw();
                        shaderProgram.UpdateUniforms(*m_scene, t_entity, nullptr);
                        t_particleSystemComponent.particleSystem->GetMesh().DrawInstanced(static_cast<int32_t>(t_particleSystemComponent.particleSystem->particles.size()), GL_TRIANGLE_STRIP);

                        resource::Mesh::EndDraw();
                    }
                    else
                    {
                        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::ParticleSystemShaderProgram>() };
                        shaderProgram.Bind();

                        m_quadMesh->InitDraw();

                        for (auto& particle : t_particleSystemComponent.particleSystem->particles)
                        {
                            // skip inactive particles
                            if (!particle.active)
                            {
                                continue;
                            }

                            // draw
                            shaderProgram.UpdateUniforms(*m_scene, t_entity, &particle);
                            m_quadMesh->DrawPrimitives(GL_TRIANGLE_STRIP);
                        }

                        resource::Mesh::EndDraw();
                    }
                }
            );

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            //OpenGl::EnableAlphaBlending();
            OpenGl::EnableAdditiveBlending();

            OpenGl::DisableWritingIntoDepthBuffer();
        }

        void FinishRendering() override
        {
            OpenGl::EnableWritingIntoDepthBuffer();
            OpenGl::DisableBlending();
        }

    protected:

    private:
        MeshSharedPtr m_quadMesh;
    };
}
