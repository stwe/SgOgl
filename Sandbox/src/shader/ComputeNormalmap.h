// This file is part of the SgOgl package.
// 
// Filename: ComputeNormalmap.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

class ComputeNormalmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::terrain::Terrain& t_terrain) override
    {
        sg::ogl::resource::TextureManager::BindForReading(t_terrain.GetHeightmapTextureId(), GL_TEXTURE0);
        SetUniform("heightmap", 0);
        SetUniform("heightmapWidth", t_terrain.GetHeightmapWidth());
        SetUniform("normalStrength", t_terrain.GetTerrainOptions().normalStrength);
    }

    std::string GetFolderName() override
    {
        return "normalmap";
    }

protected:

private:

};
