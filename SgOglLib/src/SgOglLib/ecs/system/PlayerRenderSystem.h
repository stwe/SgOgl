// This file is part of the SgOgl package.
// 
// Filename: PlayerRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "camera/ThirdPersonCamera.h"
#include "resource/shaderprogram/SkeletalModelShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/SkeletalModel.h"
#include "ecs/component/Components.h"
#include "terrain/Terrain.h"

namespace sg::ogl::ecs::system
{
    class PlayerRenderSystem : public RenderSystem<resource::shaderprogram::SkeletalModelShaderProgram>
    {
    public:
        static constexpr auto DEFAULT_TERRAIN_HEIGHT{ 0.0f };

        explicit PlayerRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void UpdateEntity(const double t_dt, const entt::entity t_entity, const uint32_t t_currentAnimation, const float t_ticksPerSecond)
        {
            const auto dt{ static_cast<float>(t_dt) };

            // get components
            auto& thirdPersonCameraComponent{ m_view.get<component::ThirdPersonCameraComponent>(t_entity) };
            auto& skeletalModelComponent{ m_view.get<component::SkeletalModelComponent>(t_entity) };
            auto& transformComponent{ m_view.get<component::TransformComponent>(t_entity) };
            //auto& healthComponent{ m_view.get<component::HealthComponent>(t_entity) };
            auto& playerComponent{ m_view.get<component::PlayerComponent>(t_entity) };

            // set current animation
            skeletalModelComponent.model->SetCurrentAnimation(t_currentAnimation);
            skeletalModelComponent.model->SetDefaultTicksPerSecond(t_ticksPerSecond);

            // handle keyboard
            if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_W))
            {
                playerComponent.currentSpeed = component::PlayerComponent::RUN_SPEED;
            }
            else if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_S))
            {
                playerComponent.currentSpeed = -component::PlayerComponent::RUN_SPEED;
            }
            else
            {
                playerComponent.currentSpeed = 0.0f;
            }

            if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_D))
            {
                playerComponent.currentTurnSpeed = -component::PlayerComponent::TURN_SPEED;
            }
            else if (m_scene->GetApplicationContext()->GetWindow().IsKeyPressed(GLFW_KEY_A))
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
            const auto dx{ static_cast<float>(distance* glm::sin(glm::radians(transformComponent.rotation.y))) };
            const auto dz{ static_cast<float>(distance* glm::cos(glm::radians(transformComponent.rotation.y))) };
            transformComponent.position.x += dx;
            transformComponent.position.z += dz;

            // update player y-position
            playerComponent.upSpeed += component::PlayerComponent::GRAVITY * dt;
            transformComponent.position.y += playerComponent.upSpeed * dt;

            // keep the player on the terrain
            if (playerComponent.terrain == nullptr)
            {
                if (transformComponent.position.y < DEFAULT_TERRAIN_HEIGHT)
                {
                    playerComponent.upSpeed = 0.0f;
                    playerComponent.isInAir = false;
                    transformComponent.position.y = DEFAULT_TERRAIN_HEIGHT;
                }
            }
            else
            {
                const auto height{ playerComponent.terrain->GetHeightAtWorldPosition(transformComponent.position.x, transformComponent.position.z) };
                if (transformComponent.position.y < height)
                {
                    playerComponent.upSpeed = 0.0f;
                    playerComponent.isInAir = false;
                    transformComponent.position.y = height;
                }
            }

            // the camera follows the player
            thirdPersonCameraComponent.thirdPersonCamera->SetPlayerPosition(transformComponent.position);
            thirdPersonCameraComponent.thirdPersonCamera->SetPlayerRotationY(transformComponent.rotation.y);
        }

        void Update(const double t_dt) override
        {
        }

        void RenderEntity(const entt::entity t_entity) const
        {
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::SkeletalModelShaderProgram>() };
            shaderProgram.Bind();

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

            resource::ShaderProgram::Unbind();
        }

        void Render() override
        {
            PrepareRendering();

            for (auto entity : m_view)
            {
                RenderEntity(entity);
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
        const entt::basic_view<
            entt::entity,
            entt::exclude_t<>,
            component::ThirdPersonCameraComponent,
            component::SkeletalModelComponent,
            component::TransformComponent,
            component::HealthComponent,
            component::PlayerComponent> m_view{
                m_scene->GetApplicationContext()->registry.view<
                component::ThirdPersonCameraComponent,
                component::SkeletalModelComponent,
                component::TransformComponent,
                component::HealthComponent,
                component::PlayerComponent>()
            };
    };
}