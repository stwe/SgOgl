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
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class ComputeSplatmap : public ShaderProgram
    {
    public:
        void UpdateUniforms(const terrain::Terrain& t_terrain) override
        {
            TextureManager::BindForReading(t_terrain.GetNormalmapTextureId(), GL_TEXTURE0);
            SetUniform("normalmap", 0);
            SetUniform("heightmapWidth", t_terrain.GetHeightmapWidth());
        }

        std::string GetFolderName() const override
        {
            return "splatmap";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
