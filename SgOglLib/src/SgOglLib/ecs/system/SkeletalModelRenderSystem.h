// This file is part of the SgOgl package.
// 
// Filename: SkeletalModelRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/SkeletalModelShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/SkeletalModel.h"
#include "ecs/component/Components.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "light/Sun.h"

namespace sg::ogl::ecs::system
{
    class SkeletalModelRenderSystem : public RenderSystem<resource::shaderprogram::SkeletalModelShaderProgram>
    {
    public:
        using PointLightContainer = std::vector<light::PointLight>;
        using DirectionalLightContainer = std::vector<light::DirectionalLight>;

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

        void Update(const double t_dt) override
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::SkeletalModelComponent, math::Transform, component::PlayerComponent>()
            };

            for (auto entity : view)
            {
                const auto dt{ static_cast<float>(t_dt) };

                auto& skeletalModelComponent{ view.get<component::SkeletalModelComponent>(entity) };
                auto& playerComponent{ view.get<component::PlayerComponent>(entity) };
                auto& transformComponent{ view.get<math::Transform>(entity) };

                skeletalModelComponent.model->SetCurrentAnimation(playerComponent.currentAnimation);
                skeletalModelComponent.model->SetDefaultTicksPerSecond(playerComponent.defaultTicksPerSecond);

                if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_UP))
                {
                    playerComponent.currentSpeed = component::PlayerComponent::RUN_SPEED;
                }
                else if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_DOWN))
                {
                    playerComponent.currentSpeed = -component::PlayerComponent::RUN_SPEED;
                }
                else
                {
                    playerComponent.currentSpeed = 0.0f;
                }

                if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_RIGHT))
                {
                    playerComponent.currentTurnSpeed = -component::PlayerComponent::TURN_SPEED;
                }
                else if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_LEFT))
                {
                    playerComponent.currentTurnSpeed = component::PlayerComponent::TURN_SPEED;
                }
                else
                {
                    playerComponent.currentTurnSpeed = 0.0f;
                }

                if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_SPACE))
                {
                    if (!playerComponent.isInAir)
                    {
                        playerComponent.upSpeed = component::PlayerComponent::JUMP_POWER;
                        playerComponent.isInAir = true;
                    }
                }

                // update player y-rotation
                transformComponent.rotation.y += playerComponent.currentTurnSpeed * dt;

                // update player x and z-position
                const auto distance = playerComponent.currentSpeed * dt;
                const auto dx{ static_cast<float>(distance * glm::sin(glm::radians(transformComponent.rotation.y))) };
                const auto dz{ static_cast<float>(distance * glm::cos(glm::radians(transformComponent.rotation.y))) };
                transformComponent.position.x += dx;
                transformComponent.position.z += dz;

                // update player y-position
                playerComponent.upSpeed += component::PlayerComponent::GRAVITY * dt;
                transformComponent.position.y += playerComponent.upSpeed * dt;

                // keep the player on the terrain/plane
                if (transformComponent.position.y < 200.0f) // todo 
                {
                    playerComponent.upSpeed = 0.0f;
                    playerComponent.isInAir = false;
                    transformComponent.position.y = 200.0f;
                }
            }
        }

        void Render() override
        {
            PointLightContainer pointLights;
            m_scene->GetApplicationContext()->registry.view<light::PointLight>().each([&pointLights](auto, auto& t_pointLight)
            {
                pointLights.push_back(t_pointLight);
            });

            DirectionalLightContainer directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkeletalModelShaderProgram>() };
            shaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::SkeletalModelComponent, math::Transform>()
            };

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
                    shaderProgram.UpdateUniforms(*m_scene, entity, *mesh, pointLights, directionalLights);
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
