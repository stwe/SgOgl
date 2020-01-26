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
#include "terrain/Terrain.h"
#include "terrain/TerrainConfig.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class ComputeSplatmap : public ShaderProgram
    {
    public:
        [[deprecated]] void UpdateUniforms(const terrain::Terrain& t_terrain) override
        {
            SetUniform("normalmap", 0);
            TextureManager::BindForReading(t_terrain.GetNormalmapTextureId(), GL_TEXTURE0);

            SetUniform("heightmapWidth", t_terrain.GetHeightmapWidth());
        }

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
