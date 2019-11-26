// This file is part of the SgOgl package.
// 
// Filename: ModelNormalmapRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

template <typename TShaderProgram>
class ModelNormalmapRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit ModelNormalmapRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::NormalmapComponent,
            sg::ogl::ecs::component::ModelComponent,
            sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            // todo
            if (modelComponent.showTriangles)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                mesh->DrawPrimitives();
                mesh->EndDraw();
            }

            // todo
            if (modelComponent.showTriangles)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
