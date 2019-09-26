#pragma once

class ModelLightingShaderProgram : public sg::ogl::resource::ShaderProgram
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

        // set ambient intensity
        SetUniform("ambientIntensity", glm::vec3(0.2f));

        // set directional light
        const auto light{ t_entity.GetParentScene()->GetDirectionalLight() };
        SetUniform("directionalLight", light);

        // set diffuse map
        SetUniform("diffuseMap", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0);
    }

protected:

private:

};
