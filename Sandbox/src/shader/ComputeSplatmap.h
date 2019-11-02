// This file is part of the SgOgl package.
// 
// Filename: ComputeSplatmap.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

class ComputeSplatmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::terrain::Terrain& t_terrain) override
    {
        sg::ogl::resource::TextureManager::BindForReading(t_terrain.GetNormalmapTextureId(), GL_TEXTURE0);
        SetUniform("normalmap", 0);
        SetUniform("heightmapWidth", t_terrain.GetHeightmapWidth());
    }

    std::string GetFolderName() override
    {
        return "splatmap";
    }

protected:

private:

};
