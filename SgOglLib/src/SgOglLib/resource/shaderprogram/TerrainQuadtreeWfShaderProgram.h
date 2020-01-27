// This file is part of the SgOgl package.
// 
// Filename: TerrainQuadtreeWfShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "resource/ShaderProgram.h"

namespace sg::ogl::resource::shaderprogram
{
    class TerrainQuadtreeWfShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {

        }

        [[nodiscard]] Options GetOptions() const override
        {
            return VERTEX_SHADER | TESSELLATION_CONTROL_SHADER | TESSELLATION_EVALUATION_SHADER | GEOMETRY_SHADER | FRAGMENT_SHADER;
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "terrain_quadtree_wireframe";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
