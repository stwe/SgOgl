// This file is part of the SgOgl package.
// 
// Filename: ModelShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "Window.h"
#include "math/Transform.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class ModelShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(
            const scene::Scene& t_scene,
            const entt::entity t_entity,
            const Mesh& t_currentMesh,
            const std::vector<light::PointLight>& t_pointLights
        ) override
        {
            auto& transformComponent{ t_scene.GetApplicationContext()->registry.get<math::Transform>(t_entity) };

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
            const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

            SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));
            SetUniform("plane", t_scene.GetCurrentClipPlane());
            SetUniform("mvpMatrix", mvp);

            if (!t_pointLights.empty())
            {
                SetUniform("numPointLights", static_cast<int32_t>(t_pointLights.size()));
                SetUniform("pointLights", t_pointLights);
            }

            SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());

            SetUniform("hasDirectionalLight", t_scene.HasDirectionalLight());
            if (t_scene.HasDirectionalLight())
            {
                SetUniform("directionalLight", t_scene.GetCurrentDirectionalLight());
            }

            SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

            SetUniform("diffuseColor", t_currentMesh.GetDefaultMaterial()->kd);
            SetUniform("hasDiffuseMap", t_currentMesh.GetDefaultMaterial()->HasDiffuseMap());
            if (t_currentMesh.GetDefaultMaterial()->HasDiffuseMap())
            {
                SetUniform("diffuseMap", 0);
                TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKd, GL_TEXTURE0);
            }

            SetUniform("specularColor", t_currentMesh.GetDefaultMaterial()->ks);
            SetUniform("hasSpecularMap", t_currentMesh.GetDefaultMaterial()->HasSpecularMap());
            if (t_currentMesh.GetDefaultMaterial()->HasSpecularMap())
            {
                SetUniform("specularMap", 1);
                TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKs, GL_TEXTURE1);
            }

            SetUniform("hasNormalMap", t_currentMesh.GetDefaultMaterial()->HasNormalMap());
            if (t_currentMesh.GetDefaultMaterial()->HasNormalMap())
            {
                SetUniform("normalMap", 2);
                TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKn, GL_TEXTURE2);
            }

            SetUniform("shininess", t_currentMesh.GetDefaultMaterial()->ns);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "model";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
