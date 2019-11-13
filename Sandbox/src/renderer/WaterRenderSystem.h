// This file is part of the SgOgl package.
// 
// Filename: WaterRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

template <typename TShaderProgram>
class WaterRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit WaterRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(const double t_dt) override
    {
        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::WaterComponent,
            sg::ogl::ecs::component::TransformComponent>();

        for (auto entity : view)
        {
            auto& waterComponent = view.get<sg::ogl::ecs::component::WaterComponent>(entity);

            waterComponent.water->moveFactor += waterComponent.water->GetWaveSpeed() * static_cast<float>(t_dt);
            waterComponent.water->moveFactor = fmod(waterComponent.water->moveFactor, 1.0f);
        }
    }

    template <typename... Args>
    void RenderReflectionTexture(Args&&... t_args)
    {
        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::WaterComponent,
            sg::ogl::ecs::component::TransformComponent>();

        sg::ogl::OpenGl::EnableClipping();

        for (auto entity : view)
        {
            auto& waterComponent = view.get<sg::ogl::ecs::component::WaterComponent>(entity);

            waterComponent.water->GetWaterFbos().BindReflectionFboAsRenderTarget();

            const auto distance{ 2.0f * (m_scene->GetCurrentCamera().GetPosition().y - waterComponent.water->GetHeight()) };
            m_scene->GetCurrentCamera().GetPosition().y -= distance;
            m_scene->GetCurrentCamera().InvertPitch();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -waterComponent.water->GetHeight()));

            (t_args->Render() , ...);

            m_scene->GetCurrentCamera().GetPosition().y += distance;
            m_scene->GetCurrentCamera().InvertPitch();

            waterComponent.water->GetWaterFbos().UnbindRenderTarget();
        }

        sg::ogl::OpenGl::DisableClipping();
        m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
    }

    template <typename... Args>
    void RenderRefractionTexture(Args&&... t_args)
    {
        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::WaterComponent,
            sg::ogl::ecs::component::TransformComponent>();

        sg::ogl::OpenGl::EnableClipping();

        for (auto entity : view)
        {
            auto& waterComponent = view.get<sg::ogl::ecs::component::WaterComponent>(entity);

            waterComponent.water->GetWaterFbos().BindRefractionFboAsRenderTarget();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, waterComponent.water->GetHeight()));

            (t_args->Render(), ...);

            waterComponent.water->GetWaterFbos().UnbindRenderTarget();
        }

        sg::ogl::OpenGl::DisableClipping();
        m_scene->SetCurrentClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f));
    }

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::WaterComponent,
            sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& meshComponent = view.get<sg::ogl::ecs::component::MeshComponent>(entity);

            meshComponent.mesh->InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
            meshComponent.mesh->DrawPrimitives();
            meshComponent.mesh->EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableFaceCulling();
        sg::ogl::OpenGl::EnableAlphaBlending();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::DisableFaceCulling();
        sg::ogl::OpenGl::DisableBlending();
    }

private:
};
