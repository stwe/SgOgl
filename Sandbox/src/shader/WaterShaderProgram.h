#pragma once

class WaterShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set mvp matrix
        const auto mvp{ projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * t_entity.GetWorldMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set textures
        SetUniform("reflectionMap", 0);
        SetUniform("refractionMap", 1);

        // get texture ids
        const auto waterComponent{ std::dynamic_pointer_cast<sg::ogl::scene::component::WaterComponent>(t_entity.GetComponentSharedPtr(sg::ogl::scene::Component::Type::WATER)) };

        // bind textures
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->reflectionTextureId, GL_TEXTURE0);
        sg::ogl::resource::TextureManager::BindForReading(waterComponent->refractionTextureId, GL_TEXTURE1);
    }

    std::string GetFolderName() override
    {
        return "water";
    }

protected:

private:

};
