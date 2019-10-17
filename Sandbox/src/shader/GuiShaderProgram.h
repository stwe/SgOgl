#pragma once

class GuiShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(sg::ogl::scene::Entity& t_entity) override
    {
        SetUniform("guiTexture", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_entity.material->mapKd, GL_TEXTURE0);

        SetUniform("transformationMatrix", GetModelMatrix(t_entity));
    }

    std::string GetFolderName() override
    {
        return "gui";
    }

protected:

private:
    glm::mat4 GetModelMatrix(sg::ogl::scene::Entity& t_entity) const
    {
        const auto translationMatrix = translate(glm::mat4(1.0f), glm::vec3(t_entity.GetLocalTransform().position.x, t_entity.GetLocalTransform().position.y, 0.0f));
        const auto scaleMatrix = scale(glm::mat4(1.0f), glm::vec3(t_entity.GetLocalTransform().scale.x, t_entity.GetLocalTransform().scale.y, 1.0f));

        return translationMatrix * scaleMatrix;
    }
};
