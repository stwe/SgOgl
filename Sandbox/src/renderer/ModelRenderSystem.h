#pragma once

template <typename TShaderProgram>
class ModelRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit ModelRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<sg::ogl::ecs::component::ModelComponent, sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                mesh->InitDraw();

                shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);

                mesh->DrawPrimitives();
                mesh->EndDraw();
            }
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override {}
    void FinishRendering() override {}

private:

};
