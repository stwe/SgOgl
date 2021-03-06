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
#include "terrain/TerrainConfig.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class ComputeNormalmap : public ShaderProgram
    {
    public:
        void UpdateUniforms(const terrain::TerrainConfig& t_terrainConfig) override
        {
            SetUniform("heightmap", 0);
            TextureManager::BindForReading(t_terrainConfig.GetHeightmapTextureId(), GL_TEXTURE0);
            TextureManager::UseBilinearFilter();

            SetUniform("heightmapWidth", t_terrainConfig.GetHeightmapWidth());
            SetUniform("normalStrength", t_terrainConfig.normalStrength);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "normalmap";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
