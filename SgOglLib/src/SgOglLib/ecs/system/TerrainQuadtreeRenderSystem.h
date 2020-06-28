// This file is part of the SgOgl package.
// 
// Filename: TerrainQuadtreeRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/TerrainQuadtreeShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "terrain/TerrainQuadtree.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class TerrainQuadtreeRenderSystem : public RenderSystem<resource::shaderprogram::TerrainQuadtreeShaderProgram>
    {
    public:
        using DirectionalLightContainer = std::vector<light::DirectionalLight>;
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit TerrainQuadtreeRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_patchMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::TERRAIN_PATCH_MESH);
            debugName = "TerrainQuadtreeRenderer";
        }

        TerrainQuadtreeRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_patchMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::TERRAIN_PATCH_MESH);
            debugName = "TerrainQuadtreeRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override
        {
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::TerrainQuadtreeComponent>()
            };

            for (auto entity : view)
            {
                auto& terrainQuadtreeComponent{ m_scene->GetApplicationContext()->registry.get<component::TerrainQuadtreeComponent>(entity) };
                terrainQuadtreeComponent.terrainQuadtree->UpdateQuadtree();
            }
        }

        void Render() override
        {
            DirectionalLightContainer directionalLights;
            m_scene->GetApplicationContext()->registry.view<light::DirectionalLight>().each([&directionalLights](auto, auto& t_directionalLight)
            {
                directionalLights.push_back(t_directionalLight);
            });

            m_scene->GetApplicationContext()->registry.view<light::Sun>().each([&directionalLights](auto, auto& t_sunLight)
            {
                directionalLights.push_back(t_sunLight);
            });

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TerrainQuadtreeShaderProgram>() };
            shaderProgram.Bind();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::TerrainQuadtreeComponent>()
            };

            for (auto entity : view)
            {
                m_patchMesh->InitDraw();

                auto& terrainQuadtreeComponent{ m_scene->GetApplicationContext()->registry.get<component::TerrainQuadtreeComponent>(entity) };
                terrainQuadtreeComponent.terrainQuadtree->Render(shaderProgram, m_patchMesh, directionalLights);

                m_patchMesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableFaceCulling();
        }

    protected:

    private:
        MeshSharedPtr m_patchMesh;
    };
}
