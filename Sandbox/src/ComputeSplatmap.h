#pragma once

class ComputeSplatmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // Get terrain component.
        const auto& terrainComponent{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain() };

        // Get heightmap width (width is same as height).
        const auto width{ terrainComponent.GetHeightmapWidth() };

        // Get splatmap texture name.
        const auto splatmapTextureName{ terrainComponent.GetTerrainOptions().splatmap.uniqueTextureName };

        // Get normalmap texture name.
        const auto normalmapTextureName{ terrainComponent.GetTerrainOptions().normalmap.uniqueTextureName };

        // Get normalmap texture id.
        const auto normalmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(normalmapTextureName) };
        SG_OGL_CORE_ASSERT(normalmapTextureId, "[ComputeSplatmap::UpdateUniforms()] Invalid normalmap texture id.")

        // Get a new texture handle by name.
        const auto splatmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(splatmapTextureName) };
        sg::ogl::resource::TextureManager::Bind(splatmapTextureId);
        sg::ogl::resource::TextureManager::UseBilinearFilter();

        // Specify the amount of storage we want to use for the new splatmap texture.
        const auto numMipmaps{ log(width) / log(2) };
        glTexStorage2D(GL_TEXTURE_2D, static_cast<int32_t>(numMipmaps), GL_RGBA32F, width, width);

        // Render to splatmap texture.
        glActiveTexture(GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(normalmapTextureId, GL_TEXTURE0);
        SetUniform("normalmap", 0);
        SetUniform("heightmapWidth", width);

        glBindImageTexture(0, splatmapTextureId, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(width / 16, width / 16, 1);
    }

protected:

private:

};
