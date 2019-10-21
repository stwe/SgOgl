#pragma once

class WaterShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set model matrix
        SetUniform("modelMatrix", t_entity.GetWorldMatrix());

        // set mvp matrix
        const auto mvp{ projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * t_entity.GetWorldMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set camera position
        SetUniform("cameraPosition", t_entity.GetParentScene()->GetCurrentCamera().GetPosition());

        // set textures
        SetUniform("reflectionMap", 0);
        SetUniform("refractionMap", 1);
        SetUniform("dudvMap", 2);
        SetUniform("normalMap", 3);
        SetUniform("depthMap", 4);

        // set light
        SetUniform("lightPosition", t_entity.GetParentScene()->GetDirectionalLight().direction);
        SetUniform("lightColor", t_entity.GetParentScene()->GetDirectionalLight().diffuseIntensity);

        // get water component
        const auto waterComponent{ std::dynamic_pointer_cast<sg::ogl::scene::component::WaterComponent>(t_entity.GetComponentSharedPtr(sg::ogl::scene::Component::Type::WATER)) };

        // set move factor
        SetUniform("moveFactor", waterComponent->moveFactor);

        // bind textures
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->reflectionTextureId, GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->refractionTextureId, GL_TEXTURE1);
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->dudvTextureId, GL_TEXTURE2);
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->normalTextureId, GL_TEXTURE3);
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->depthTextureId, GL_TEXTURE4);
    }

    std::string GetFolderName() override
    {
        return "water";
    }

protected:

private:

};
