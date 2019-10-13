#pragma once

class ComputeNormalmap : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // Get terrain.
        const auto& terrain{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain() };

        // Set uniforms.
        sg::ogl::resource::TextureManager::BindForReading(terrain.GetHeightmapTextureId(), GL_TEXTURE0);
        SetUniform("heightmap", 0);
        SetUniform("heightmapWidth", terrain.GetHeightmapWidth());
        SetUniform("normalStrength", terrain.GetTerrainOptions().normalStrength);
    }

protected:

private:

};
