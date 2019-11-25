// This file is part of the SgOgl package.
// 
// Filename: InstancingShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

class InstancingShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& modelComponent{ t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::ModelComponent>(t_entity) };

        SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
        SetUniform("viewMatrix", t_scene.GetCurrentCamera().GetViewMatrix());

        SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());
        SetUniform("directionalLight", t_scene.GetDirectionalLight());
        SetUniform("pointLight", t_scene.GetPointLight());

        SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

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

        SetUniform("shininess", t_currentMesh.GetDefaultMaterial()->ns);
        SetUniform("fakeNormals", modelComponent.fakeNormals);
    }

    std::string GetFolderName() override
    {
        return "instancing";
    }

protected:

private:

};
