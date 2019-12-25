// This file is part of the SgOgl package.
// 
// Filename: TerrainRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/TerrainShaderProgram.h"

namespace sg::ogl::ecs::system
{
    class TerrainRenderSystem : public RenderSystem<resource::shaderprogram::TerrainShaderProgram>
    {
    public:
        explicit TerrainRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {}

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view = m_scene->GetApplicationContext()->registry.view<
                component::TerrainComponent,
                component::TransformComponent>();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TerrainShaderProgram>() };

            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& terrainComponent = view.get<component::TerrainComponent>(entity);

                terrainComponent.terrain->GetMesh().InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, terrainComponent.terrain->GetMesh());
                terrainComponent.terrain->GetMesh().DrawPrimitives();
                terrainComponent.terrain->GetMesh().EndDraw();
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableFaceCulling();
        }

    private:

    };
}
