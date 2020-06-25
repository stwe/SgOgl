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
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit SkeletalModelRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            debugName = "SkeletalModelRenderer";
        }

        SkeletalModelRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            debugName = "SkeletalModelRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

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
            /*
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::SkeletalModelComponent, math::Transform>()
            };

            for (auto entity : view)
            {
                auto& skeletalModelComponent{ view.get<component::SkeletalModelComponent>(entity) };

                skeletalModelComponent.model->SetCurrentAnimation(0);
                skeletalModelComponent.model->SetDefaultTicksPerSecond(1200.0f);
            }
            */
        }

        void Render() override
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::SkeletalModelComponent, math::Transform>()
            };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkeletalModelShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& skeletalModelComponent{ view.get<component::SkeletalModelComponent>(entity) };

                if (skeletalModelComponent.showTriangles)
                {
                    OpenGl::EnableWireframeMode();
                }

                for (auto& mesh : skeletalModelComponent.model->GetMeshes())
                {
                    mesh->InitDraw();
                    shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                    mesh->DrawPrimitives();
                    mesh->EndDraw();
                }

                if (skeletalModelComponent.showTriangles)
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
