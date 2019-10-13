#pragma once

class DomeShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // get projection matrix
        const auto projectionMatrix{ t_entity.GetParentScene()->GetApplicationContext()->GetWindow()->GetProjectionMatrix() };

        // calc mvp matrix
        const auto mvp{ projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * t_entity.GetWorldMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set world matrix
        SetUniform("worldMatrix", t_entity.GetWorldMatrix());
    }

    std::string GetFolderName() override
    {
        return "dome";
    }

protected:

private:

};
