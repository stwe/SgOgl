#pragma once

class ModelNormalmapShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& transformComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TransformComponent>(t_entity);

        const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
        const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

        SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));
        SetUniform("mvpMatrix", mvp);
        SetUniform("plane", t_scene.GetCurrentClipPlane());

        SetUniform("directionalLightDirection", t_scene.GetDirectionalLight().direction);
        SetUniform("pointLightPosition", t_scene.GetPointLight().position);
        SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

        SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());
        SetUniform("directionalLight", t_scene.GetDirectionalLight());
        SetUniform("pointLight", t_scene.GetPointLight());

        SetUniform("diffuseColor", t_currentMesh.GetDefaultMaterial()->kd);
        SetUniform("hasDiffuseMap", t_currentMesh.GetDefaultMaterial()->HasDiffuseMap());
        if (t_currentMesh.GetDefaultMaterial()->HasDiffuseMap())
        {
            SetUniform("diffuseMap", 0);
            sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKd, GL_TEXTURE0);
        }

        SetUniform("specularColor", t_currentMesh.GetDefaultMaterial()->ks);
        SetUniform("hasSpecularMap", t_currentMesh.GetDefaultMaterial()->HasSpecularMap());
        if (t_currentMesh.GetDefaultMaterial()->HasSpecularMap())
        {
            SetUniform("specularMap", 1);
            sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKs, GL_TEXTURE1);
        }

        
        SetUniform("normalMap", 2);
        sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapBump, GL_TEXTURE2);

        SetUniform("shininess", t_currentMesh.GetDefaultMaterial()->ns);
    }

    std::string GetFolderName() override
    {
        return "model_normalmap";
    }

protected:

private:

};
