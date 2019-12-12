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
#include "ecs/component/SkeletalModelComponent.h"

namespace sg::ogl::ecs::system
{
    class SkeletalModelRenderSystem : public RenderSystem<resource::shaderprogram::SkeletalModelShaderProgram>
    {
    public:
        explicit SkeletalModelRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override
        {
        }

        void Render() override
        {
            PrepareRendering();

            auto view{ m_scene->GetApplicationContext()->registry.view<component::SkeletalModelComponent, component::TransformComponent>() };
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkeletalModelShaderProgram>() };

            for (auto entity : view)
            {
                auto& skeletalModelComponent{ view.get<component::SkeletalModelComponent>(entity) };

                //auto& transformComponent{ m_scene->GetApplicationContext()->registry.get<component::TransformComponent>(entity) };
                //transformComponent.position.y += 0.005f;

                shaderProgram.Bind();

                ////////////////////

                std::vector<aiMatrix4x4> transforms;
                skeletalModelComponent.model->BoneTransform(glfwGetTime(), transforms);
                shaderProgram.SetUniform("bones", transforms);

                ////////////////////

                if (skeletalModelComponent.showTriangles)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                resource::ShaderProgram::Unbind();
            }

            FinishRendering();
        }

    protected:
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

    private:

    };
}
