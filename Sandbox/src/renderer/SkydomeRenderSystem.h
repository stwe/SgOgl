#pragma once

template <typename TShaderProgram>
class SkydomeRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit SkydomeRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::ModelComponent,
            sg::ogl::ecs::component::TransformComponent,
            sg::ogl::ecs::component::SkydomeComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            modelComponent.model->GetMeshes()[0]->InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, *modelComponent.model->GetMeshes()[0]);
            modelComponent.model->GetMeshes()[0]->DrawPrimitives();
            modelComponent.model->GetMeshes()[0]->EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override {}
    void FinishRendering() override {}

private:

};
