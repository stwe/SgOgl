#pragma once

class ComputeSplatmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // Get terrain component.
        const auto& terrain{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain() };

        // Get normalmap texture name.
        const auto normalmapTextureName{ terrain.GetTerrainOptions().normalmap.uniqueTextureName };

        // Get normalmap texture id.
        const auto normalmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(normalmapTextureName) };
        SG_OGL_CORE_ASSERT(normalmapTextureId, "[ComputeSplatmap::UpdateUniforms()] Invalid normalmap texture id.")

        // Set uniforms.
        sg::ogl::resource::TextureManager::BindForReading(normalmapTextureId, GL_TEXTURE0);
        SetUniform("normalmap", 0);
        SetUniform("heightmapWidth", terrain.GetHeightmapWidth());
    }

protected:

private:

};
