// This file is part of the SgOgl package.
// 
// Filename: ComputeNormalmap.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "terrain/Terrain.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class ComputeNormalmap : public ShaderProgram
    {
    public:
        void UpdateUniforms(const terrain::Terrain& t_terrain) override
        {
            TextureManager::BindForReading(t_terrain.GetHeightmapTextureId(), GL_TEXTURE0);
            SetUniform("heightmap", 0);
            SetUniform("heightmapWidth", t_terrain.GetHeightmapWidth());
            SetUniform("normalStrength", t_terrain.GetTerrainOptions().normalStrength);
        }

        std::string GetFolderName() const override
        {
            return "normalmap";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
