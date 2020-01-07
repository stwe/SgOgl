// This file is part of the SgOgl package.
// 
// Filename: SkyboxShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "Window.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class SkyboxShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
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
            auto& cubemapComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::CubemapComponent>(t_entity) };

            // bind cubemap
            TextureManager::BindForReading(cubemapComponent.cubemapId, GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "skybox";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
