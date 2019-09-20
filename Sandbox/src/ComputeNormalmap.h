#pragma once

#include <cmath>

class ComputeNormalmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // Get terrain component.
        const auto& terrainComponent{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain() };

        // Get heightmap width (width is same as height).
        const auto width{ terrainComponent.GetHeightmapWidth() };

        // Get heightmap texture id.
        const auto heightmapTextureId{ terrainComponent.GetHeightmapTextureId() };

        // Get normal strength.
        const auto strength{ terrainComponent.GetTerrainOptions().normalStrength };

        // Get normalmap texture name.
        const auto normalmapTextureName{ terrainComponent.GetTerrainOptions().normalmap.uniqueTextureName };

        // Get a new texture handle by name.
        const auto normalmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(normalmapTextureName) };
        sg::ogl::resource::TextureManager::Bind(normalmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();

        // Specify the amount of storage we want to use for the new normalmap texture.
        const auto numMipmaps{ log(width) / log(2) };
        glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, width, width);

        // Render to normalmap texture.
        glActiveTexture(GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(heightmapTextureId, GL_TEXTURE0);
        SetUniform("heightmap", 0);
        SetUniform("heightmapWidth", width);
        SetUniform("normalStrength", strength);

        glBindImageTexture(0, normalmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(width / 16, width / 16, 1);
    }

protected:

private:

};
