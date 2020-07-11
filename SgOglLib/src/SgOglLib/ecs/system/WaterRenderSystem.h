// This file is part of the SgOgl package.
// 
// Filename: WaterRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "OpenGl.h"
#include "ecs/component/Components.h"
#include "resource/Mesh.h"
#include "resource/ModelManager.h"
#include "resource/shaderprogram/WaterShaderProgram.h"
#include "resource/ShaderManager.h"
#include "math/Transform.h"
#include "light/DirectionalLight.h"
#include "light/Sun.h"
#include "scene/Scene.h"

namespace sg::ogl::ecs::system
{
    class WaterRenderSystem : public RenderSystem<resource::shaderprogram::WaterShaderProgram>
    {
    public:
        using DirectionalLightContainer = std::vector<light::DirectionalLight>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit WaterRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_waterMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::WATER_MESH);
            name = "WaterRenderer";
        }

        WaterRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_waterMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::WATER_MESH);
            name = "WaterRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(const double t_dt) override
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::WaterComponent, math::Transform>()
            };

            for (auto entity : view)
            {
                auto& waterComponent{ view.get<component::WaterComponent>(entity) };
                auto& transformComponent{ view.get<math::Transform>(entity) };

                waterComponent.water->moveFactor += waterComponent.water->GetWaveSpeed() * static_cast<float>(t_dt);
                waterComponent.water->moveFactor = fmod(waterComponent.water->moveFactor, 1.0f);

                transformComponent.position.x = waterComponent.water->GetXPosition();
                transformComponent.position.y = waterComponent.water->GetHeight();
                transformComponent.position.z = waterComponent.water->GetZPosition();
                transformComponent.scale = waterComponent.water->GetTileSize();
            }
        }

        void RenderReflectionTexture()
        {
            OpenGl::EnableClipping();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::WaterComponent, math::Transform>()
            };

            for (auto entity : view)
            {
                auto& waterComponent{ view.get<component::WaterComponent>(entity) };
                waterComponent.water->GetWaterFbos().BindReflectionFboAsRenderTarget();

                const auto distance{ 2.0f * (m_scene->GetCurrentCamera().GetPosition().y - waterComponent.water->GetHeight()) };
                m_scene->GetCurrentCamera().GetPosition().y -= distance;
                m_scene->GetCurrentCamera().InvertPitch();
                m_scene->GetCurrentCamera().Update(0.016);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                m_scene->SetCurrentClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -waterComponent.water->GetHeight()));

                for (auto* renderer : waterComponent.water->toReflectionTexture)
                {
                    renderer->PrepareRendering();
                    renderer->Render();
                    renderer->FinishRendering();
                }

                m_scene->GetCurrentCamera().GetPosition().y += distance;
                m_scene->GetCurrentCamera().InvertPitch();
                m_scene->GetCurrentCamera().Update(0.016);

                waterComponent.water->GetWaterFbos().UnbindRenderTarget();
            }

            OpenGl::DisableClipping();

            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
        }

        void RenderRefractionTexture()
        {
            OpenGl::EnableClipping();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::WaterComponent, math::Transform>()
            };

            for (auto entity : view)
            {
                auto& waterComponent{ view.get<component::WaterComponent>(entity) };
                waterComponent.water->GetWaterFbos().BindRefractionFboAsRenderTarget();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, waterComponent.water->GetHeight()));

                for (auto* renderer : waterComponent.water->toRefractionTexture)
                {
                    renderer->PrepareRendering();
                    renderer->Render();
                    renderer->FinishRendering();
                }

                waterComponent.water->GetWaterFbos().UnbindRenderTarget();
            }

            OpenGl::DisableClipping();

            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
        }

        void Render() override
        {
            DirectionalLightContainer directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto t_entity, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto t_entity, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::WaterShaderProgram>() };
            shaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::WaterComponent, math::Transform>()
            };

            for (auto entity : view)
            {
                m_waterMesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *m_waterMesh, {}, directionalLights);
                m_waterMesh->DrawPrimitives();
                m_waterMesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableFaceCulling();
            OpenGl::EnableAlphaBlending();
        }

        void FinishRendering() override
        {
            OpenGl::DisableFaceCulling();
            OpenGl::DisableBlending();
        }

    protected:

    private:
        MeshSharedPtr m_waterMesh;
    };
}
