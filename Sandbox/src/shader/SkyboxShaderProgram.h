// This file is part of the SgOgl package.
// 
// Filename: SkyboxShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

class SkyboxShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        // remove translation from the view matrix
        const auto skyboxViewMatrix{ glm::mat4(glm::mat3(t_scene.GetCurrentCamera().GetViewMatrix())) };

        // get projection matrix
        const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };

        // set shader uniforms
        SetUniform("projectionMatrix", projectionMatrix);
        SetUniform("viewMatrix", skyboxViewMatrix);
        SetUniform("cubeSampler", 0);

        // get cubemap component
        auto& cubemapComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::CubemapComponent>(t_entity);

        // bind cubemap
        sg::ogl::resource::TextureManager::BindForReading(cubemapComponent.cubemapId, GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
    }

    std::string GetFolderName() override
    {
        return "skybox";
    }

protected:

private:

};
