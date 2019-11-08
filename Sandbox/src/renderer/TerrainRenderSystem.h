// This file is part of the SgOgl package.
// 
// Filename: TerrainRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

template <typename TShaderProgram>
class TerrainRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit TerrainRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::TerrainComponent,
            sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& terrainComponent = view.get<sg::ogl::ecs::component::TerrainComponent>(entity);

            terrainComponent.terrain->GetMesh().InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, terrainComponent.terrain->GetMesh());
            terrainComponent.terrain->GetMesh().DrawPrimitives();
            terrainComponent.terrain->GetMesh().EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override {}
    void FinishRendering() override {}

private:

};
