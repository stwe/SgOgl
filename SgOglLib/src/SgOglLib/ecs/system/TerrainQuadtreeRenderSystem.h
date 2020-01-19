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
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        explicit TerrainQuadtreeRenderSystem(scene::Scene* t_scene)
            : RenderSystem(t_scene)
        {
            m_patchMesh = m_scene->GetApplicationContext()->GetModelManager().GetStaticMeshByName(resource::ModelManager::TERRAIN_PATCH_MESH);
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            PrepareRendering();

            auto view{ m_scene->GetApplicationContext()->registry.view<
                component::TerrainQuadtreeComponent>()
            };

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TerrainQuadtreeShaderProgram>() };
            shaderProgram.Bind();

            for (auto entity : view)
            {
                auto& terrainQuadtreeComponent{ m_scene->GetApplicationContext()->registry.get<component::TerrainQuadtreeComponent>(entity) };
                terrainQuadtreeComponent.terrainQuadtree->Render(shaderProgram, m_patchMesh);
            }

            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
        void PrepareRendering() override
        {
            //OpenGl::EnableWireframeMode();
            //OpenGl::EnableAlphaBlending();
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            //OpenGl::DisableWireframeMode();
            //OpenGl::DisableBlending();
            OpenGl::DisableFaceCulling();
        }

    private:
        MeshSharedPtr m_patchMesh;
    };
}
