#pragma once

class TerrainShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get terrain options
        const auto& terrainOptions{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain().GetTerrainOptions() };

        sg::ogl::math::Transform transform;
        transform.position = glm::vec3(terrainOptions.xPos, 0.0f, terrainOptions.zPos);

        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // calc mvp matrix
        const auto mvp{ projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * transform.GetModelMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set textures
        int32_t counter{ 0 };
        for (const auto& entry : terrainOptions.texturePack)
        {
            SetUniform(entry.first, counter);
            sg::ogl::resource::TextureManager::BindForReading(t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
            counter++;
        }

        // bind normalmap
        const auto normalmapTextureName{ terrainOptions.normalmap.uniqueTextureName };
        const auto normalmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(normalmapTextureName) };
        SetUniform("normalmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(normalmapTextureId, GL_TEXTURE0 + counter);
        counter++;

        // bind splatmap
        const auto splatmapTextureName{ terrainOptions.splatmap.uniqueTextureName };
        const auto splatmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(splatmapTextureName) };
        SetUniform("splatmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(splatmapTextureId, GL_TEXTURE0 + counter);
    }

protected:

private:

};
