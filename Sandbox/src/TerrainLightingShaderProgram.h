#pragma once

class TerrainLightingShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get terrain options
        const auto& terrainOptions{ dynamic_cast<sg::ogl::scene::TerrainComponent*>(&t_entity.GetComponent(sg::ogl::scene::Component::Type::TERRAIN))->GetTerrain().GetTerrainOptions() };

        // set the position of the terrain
        sg::ogl::math::Transform transform;
        transform.position = glm::vec3(terrainOptions.xPos, 0.0f, terrainOptions.zPos);

        // get model matrix
        const auto modelMatrix{ transform.GetModelMatrix() };

        // get view matrix
        const auto viewMatrix{ t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() };

        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set model matrix
        SetUniform("modelMatrix", modelMatrix);

        // set vp matrix
        SetUniform("vpMatrix", projectionMatrix * viewMatrix);

        // set camera position
        SetUniform("cameraPosition", t_entity.GetParentScene()->GetCurrentCamera().GetPosition());

        // set directional light
        auto light{ t_entity.GetParentScene()->GetDirectionalLight() };
        SetUniform("directionalLight", light);

        // set and bind textures
        int32_t counter{ 0 };
        for (const auto& entry : terrainOptions.texturePack)
        {
            SetUniform(entry.first, counter);
            sg::ogl::resource::TextureManager::BindForReading(t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
            counter++;
        }

        // set and bind normalmap
        const auto normalmapTextureName{ terrainOptions.normalmap.uniqueTextureName };
        const auto normalmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(normalmapTextureName) };
        SetUniform("normalmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(normalmapTextureId, GL_TEXTURE0 + counter);
        counter++;

        // set and bind splatmap
        const auto splatmapTextureName{ terrainOptions.splatmap.uniqueTextureName };
        const auto splatmapTextureId{ t_entity.GetParentScene()->GetApplicationContext()->GetTextureManager()->GetTextureId(splatmapTextureName) };
        SetUniform("splatmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(splatmapTextureId, GL_TEXTURE0 + counter);
    }

protected:

private:

};

