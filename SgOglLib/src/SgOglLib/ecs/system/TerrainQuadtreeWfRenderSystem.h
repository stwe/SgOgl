// This file is part of the SgOgl package.
// 
// Filename: TerrainQuadtreeWfRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "RenderSystem.h"
#include "resource/shaderprogram/TerrainQuadtreeWfShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/Mesh.h"
#include "terrain/TerrainQuadtree.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class TerrainQuadtreeWfRenderSystem : public RenderSystem<resource::shaderprogram::TerrainQuadtreeWfShaderProgram>
    {
    public:
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit TerrainQuadtreeWfRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_patchMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::TERRAIN_PATCH_MESH);
            name = "TerrainQuadtreeWfRenderer";
        }

        TerrainQuadtreeWfRenderSystem(const int t_priority, scene::Scene* t_scene)
            : RenderSystem(t_priority, t_scene)
        {
            m_patchMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::TERRAIN_PATCH_MESH);
            name = "TerrainQuadtreeWfRenderer";
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
            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::TerrainQuadtreeComponent>()
            };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TerrainQuadtreeWfShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                m_patchMesh->InitDraw();

                auto& terrainQuadtreeComponent{ m_scene->GetApplicationContext()->registry.get<component::TerrainQuadtreeComponent>(entity) };
                terrainQuadtreeComponent.terrainQuadtree->RenderWireframe(shaderProgram, m_patchMesh);

                m_patchMesh->EndDraw();
            }

            resource::ShaderProgram::Unbind();
        }

        void PrepareRendering() override
        {
            OpenGl::EnableWireframeMode();
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableWireframeMode();
            OpenGl::DisableFaceCulling();
        }

    protected:

    private:
        MeshSharedPtr m_patchMesh;
    };
}
