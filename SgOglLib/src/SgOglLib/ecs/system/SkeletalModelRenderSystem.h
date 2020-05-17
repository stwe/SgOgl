// This file is part of the SgOgl package.
// 
// Filename: SkeletalModelRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/SkeletalModelShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/SkeletalModel.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class SkeletalModelRenderSystem : public RenderSystem<resource::shaderprogram::SkeletalModelShaderProgram>
    {
    public:
        explicit SkeletalModelRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void UpdateEntity(const double t_dt, const entt::entity t_entity, const uint32_t t_currentAnimation, const float t_ticksPerSecond)
        {
            /*
            auto& skeletalModelComponent{ m_view.get<component::SkeletalModelComponent>(t_entity) };

            skeletalModelComponent.model->SetCurrentAnimation(t_currentAnimation);
            skeletalModelComponent.model->SetDefaultTicksPerSecond(t_ticksPerSecond);
            */
        }

        void Update(const double t_dt) override
        {
        }

        void RenderEntity(const entt::entity t_entity) const
        {
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkeletalModelShaderProgram>() };
            shaderProgram.Bind();

            /*
            auto& skeletalModelComponent{ m_view.get<component::SkeletalModelComponent>(t_entity) };

            if (skeletalModelComponent.showTriangles)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            for (auto& mesh : skeletalModelComponent.model->GetMeshes())
            {
                mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, t_entity, *mesh);
                mesh->DrawPrimitives();
                mesh->EndDraw();
            }

            if (skeletalModelComponent.showTriangles)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            */
            resource::ShaderProgram::Unbind();
        }

        void Render() override
        {
            /*
            for (auto entity : m_view)
            {
                RenderEntity(entity);
            }
            */
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
        /*
        const entt::basic_view<
            entt::entity,
            entt::exclude_t<component::PlayerComponent>,
            component::SkeletalModelComponent,
            component::TransformComponent> m_view{
                m_scene->GetApplicationContext()->registry.view<
                component::SkeletalModelComponent,
                component::TransformComponent>(entt::exclude<component::PlayerComponent>)
            };*/
    };
}
