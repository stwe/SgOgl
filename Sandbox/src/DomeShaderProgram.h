#pragma once

class DomeShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:

    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        // calc mvp matrix
        const auto mvp{ t_entity.GetParentScene()->projectionMatrix * t_entity.GetParentScene()->GetCurrentCamera().GetViewMatrix() * t_entity.GetWorldMatrix() };
        SetUniform("mvpMatrix", mvp);

        // set world matrix
        SetUniform("worldMatrix", t_entity.GetWorldMatrix());
    }

protected:

private:

};
