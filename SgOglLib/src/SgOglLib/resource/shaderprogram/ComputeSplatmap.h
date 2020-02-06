// This file is part of the SgOgl package.
// 
// Filename: ComputeSplatmap.h
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
    class ComputeSplatmap : public ShaderProgram
    {
    public:
        void UpdateUniforms(const terrain::TerrainConfig& t_terrainConfig) override
        {
            SetUniform("normalmap", 0);
            TextureManager::BindForReading(t_terrainConfig.GetNormalmapTextureId(), GL_TEXTURE0);
            TextureManager::UseBilinearFilter();

            SetUniform("heightmapWidth", t_terrainConfig.GetHeightmapWidth());
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "splatmap";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
