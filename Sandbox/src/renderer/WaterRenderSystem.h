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

            waterComponent.moveFactor += waterComponent.waveSpeed * static_cast<float>(t_dt);
            waterComponent.moveFactor = fmod(waterComponent.moveFactor, 1.0f);
        }
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
