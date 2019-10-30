#pragma once

template <typename TShaderProgram>
class SkyboxRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit SkyboxRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::CubemapComponent>();

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
        sg::ogl::OpenGl::SetDepthFunc(GL_LEQUAL);
    }

    void FinishRendering() override
    {
        // GL_LESS is the initial depth comparison function
        sg::ogl::OpenGl::SetDepthFunc(GL_LESS);
    }

private:

};
