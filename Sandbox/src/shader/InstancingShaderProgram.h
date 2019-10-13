#pragma once

class InstancingShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set projection matrix
        SetUniform("projectionMatrix", projectionMatrix);

        // set view matrix
        SetUniform("viewMatrix", t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix());

        // set diffuse map
        SetUniform("diffuseMap", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0);

        // set ambient intensity
        SetUniform("ambientIntensity", glm::vec3(1.0f));
    }

protected:

private:

};
