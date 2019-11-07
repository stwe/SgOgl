// This file is part of the SgOgl package.
// 
// Filename: InstancingRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

template <typename TShaderProgram>
class InstancingRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit InstancingRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::InstancesComponent,
            sg::ogl::ecs::component::ModelComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& instancesComponent = view.get<sg::ogl::ecs::component::InstancesComponent>(entity);
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                mesh->DrawInstanced(instancesComponent.instances);
                mesh->EndDraw();
            }
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableAlphaBlending();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::DisableBlending();
    }

private:

};
