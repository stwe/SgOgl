#pragma once

class DepthMapShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        SetUniform("guiTexture", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0);

        auto mat{ glm::mat4(1.0f) };
        mat = translate(mat, glm::vec3(t_entity.GetLocalTransform().position.x, t_entity.GetLocalTransform().position.y, 0.0f));
        mat = scale(mat, glm::vec3(t_entity.GetLocalTransform().scale.x, t_entity.GetLocalTransform().scale.y, 1.0f));

        SetUniform("transformationMatrix", mat);

        SetUniform("near_plane", t_entity.GetParentScene()->GetApplicationContext()->GetProjectionOptions().nearPlane);
        SetUniform("far_plane", t_entity.GetParentScene()->GetApplicationContext()->GetProjectionOptions().farPlane);
    }

    std::string GetFolderName() override
    {
        return "gui_depth_map";
    }

protected:

private:

};
