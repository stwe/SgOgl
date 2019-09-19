#pragma once

class TerrainShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        /*
        // get terrain options
        const auto& terrainOptions{ t_terrain.GetTerrainOptions() };

        // set transform
        math::Transform transform;
        transform.position = glm::vec3(terrainOptions.xPos, 0.0f, terrainOptions.zPos);
        const auto mvp{ m_projectionMatrix * m_parentScene->GetCurrentCamera().GetViewMatrix() * transform.GetModelMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // set textures
        int32_t counter{ 0 };
        for (const auto& entry : terrainOptions.texturePack)
        {
            shaderProgram->SetUniform(entry.first, counter);
            resource::TextureManager::BindForReading(m_textureManager.GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
            counter++;
        }

        // bind normalmap
        shaderProgram->SetUniform("normalmap", counter);
        resource::TextureManager::BindForReading(terrainOptions.normalmap.GetTextureId(), GL_TEXTURE0 + counter);
        counter++;

        // bind splatmap
        shaderProgram->SetUniform("splatmap", counter);
        resource::TextureManager::BindForReading(terrainOptions.splatmap.GetTextureId(), GL_TEXTURE0 + counter);
        */
    }

protected:

private:

};
