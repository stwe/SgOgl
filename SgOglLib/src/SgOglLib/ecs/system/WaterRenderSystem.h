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
#include "ecs/component/MeshComponent.h"
#include "ecs/component/WaterComponent.h"
#include "ecs/component/TransformComponent.h"

namespace sg::ogl::ecs::system
{
    template <typename TShaderProgram>
    class WaterRenderSystem : public RenderSystem<TShaderProgram>
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit WaterRenderSystem(scene::Scene* t_scene)
            : RenderSystem<TShaderProgram>(t_scene)
        {}

        WaterRenderSystem(const WaterRenderSystem& t_other) = delete;
        WaterRenderSystem(WaterRenderSystem&& t_other) noexcept = delete;
        WaterRenderSystem& operator=(const WaterRenderSystem& t_other) = delete;
        WaterRenderSystem& operator=(WaterRenderSystem&& t_other) noexcept = delete;

        virtual ~WaterRenderSystem() noexcept = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(const double t_dt) override
        {
            auto view = m_scene->GetApplicationContext()->registry.view<
                component::MeshComponent,
                component::WaterComponent,
                component::TransformComponent>();

            for (auto entity : view)
            {
                auto& waterComponent = view.get<component::WaterComponent>(entity);

                waterComponent.water->moveFactor += waterComponent.water->GetWaveSpeed() * static_cast<float>(t_dt);
                waterComponent.water->moveFactor = fmod(waterComponent.water->moveFactor, 1.0f);
            }
        }

        template <typename... Args>
        void RenderReflectionTexture(Args&&... t_args)
        {
            auto view = m_scene->GetApplicationContext()->registry.view<
                component::MeshComponent,
                component::WaterComponent,
                component::TransformComponent>();

            OpenGl::EnableClipping();

            for (auto entity : view)
            {
                auto& waterComponent = view.get<component::WaterComponent>(entity);

                waterComponent.water->GetWaterFbos().BindReflectionFboAsRenderTarget();

                const auto distance{ 2.0f * (m_scene->GetCurrentCamera().GetPosition().y - waterComponent.water->GetHeight()) };
                m_scene->GetCurrentCamera().GetPosition().y -= distance;
                m_scene->GetCurrentCamera().InvertPitch();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                m_scene->SetCurrentClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -waterComponent.water->GetHeight()));

                (t_args->Render(), ...);

                m_scene->GetCurrentCamera().GetPosition().y += distance;
                m_scene->GetCurrentCamera().InvertPitch();

                waterComponent.water->GetWaterFbos().UnbindRenderTarget();
            }

            OpenGl::DisableClipping();
            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
        }

        template <typename... Args>
        void RenderRefractionTexture(Args&&... t_args)
        {
            auto view = m_scene->GetApplicationContext()->registry.view<
                component::MeshComponent,
                component::WaterComponent,
                component::TransformComponent>();

            OpenGl::EnableClipping();

            for (auto entity : view)
            {
                auto& waterComponent = view.get<component::WaterComponent>(entity);

                waterComponent.water->GetWaterFbos().BindRefractionFboAsRenderTarget();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, waterComponent.water->GetHeight()));

                (t_args->Render(), ...);

                waterComponent.water->GetWaterFbos().UnbindRenderTarget();
            }

            OpenGl::DisableClipping();
            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
        }

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::MeshComponent,
                component::WaterComponent,
                component::TransformComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& meshComponent = view.get<component::MeshComponent>(entity);

                meshComponent.mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
                meshComponent.mesh->DrawPrimitives();
                meshComponent.mesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:

    private:
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
    };
}
