#pragma once

class ModelShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set mvp matrix
        const auto mvp{ projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * t_entity.GetWorldMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set diffuse color
        SetUniform("diffuseColor", t_entity.material->kd);

        // set has diffuse map
        SetUniform("hasDiffuseMap", t_entity.material->HasDiffuseMap());

        // set diffuse map
        if (t_entity.material->HasDiffuseMap())
        {
            SetUniform("diffuseMap", 0);
            sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0);
        }

        // set ambient intensity
        SetUniform("ambientIntensity", glm::vec3(1.0f));
    }

    std::string GetFolderName() override
    {
        return "model";
    }

protected:

private:

};
