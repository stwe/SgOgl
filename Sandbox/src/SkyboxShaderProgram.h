#pragma once

class SkyboxShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // remove translation from the view matrix
        const auto skyboxViewMatrix{ glm::mat4(glm::mat3(t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix())) };

        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // set shader uniforms
        SetUniform("projectionMatrix", projectionMatrix);
        SetUniform("viewMatrix", skyboxViewMatrix);
        SetUniform("cubeSampler", 0);

        // bind cubemap saved in mapKd
        sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
    }

protected:

private:

};
