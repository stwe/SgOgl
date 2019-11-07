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
        SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
        SetUniform("viewMatrix", t_scene.GetCurrentCamera().GetViewMatrix());
        SetUniform("ambientIntensity", glm::vec3(1.0f));
        SetUniform("diffuseMap", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKd, GL_TEXTURE0);
    }

    std::string GetFolderName() override
    {
        return "instancing";
    }

protected:

private:

};
