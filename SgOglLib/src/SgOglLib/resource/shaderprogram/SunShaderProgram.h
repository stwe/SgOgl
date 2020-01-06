// This file is part of the SgOgl package.
// 
// Filename: SunShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "light/Sun.h"
#include "scene/Scene.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class SunShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& sunComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::SunComponent>(t_entity) };
            const auto sunPosition{ sunComponent.sun->GetWorldPosition(t_scene.GetCurrentCamera().GetPosition()) };

            auto modelMatrix{ glm::mat4(1.0f) };
            modelMatrix = translate(modelMatrix, sunPosition);
            auto mvMatrix{ ApplyViewMatrix(modelMatrix, t_scene.GetCurrentCamera().GetViewMatrix()) };
            mvMatrix = scale(mvMatrix, glm::vec3(sunComponent.sun->scale));

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };

            SetUniform("mvpMatrix", projectionMatrix * mvMatrix);

            SetUniform("sunTexture", 0);
            TextureManager::BindForReading(sunComponent.sun->textureId, GL_TEXTURE0);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "sun";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:
        static glm::mat4 ApplyViewMatrix(glm::mat4& t_modelMatrix, glm::mat4&& t_viewMatrix)
        {
            t_modelMatrix[0][0] = t_viewMatrix[0][0];
            t_modelMatrix[0][1] = t_viewMatrix[1][0];
            t_modelMatrix[0][2] = t_viewMatrix[2][0];
            t_modelMatrix[1][0] = t_viewMatrix[0][1];
            t_modelMatrix[1][1] = t_viewMatrix[1][1];
            t_modelMatrix[1][2] = t_viewMatrix[2][1];
            t_modelMatrix[2][0] = t_viewMatrix[0][2];
            t_modelMatrix[2][1] = t_viewMatrix[1][2];
            t_modelMatrix[2][2] = t_viewMatrix[2][2];

            return t_viewMatrix * t_modelMatrix;
        }
    };
}
