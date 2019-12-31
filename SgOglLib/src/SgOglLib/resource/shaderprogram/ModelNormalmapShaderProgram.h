// This file is part of the SgOgl package.
// 
// Filename: ModelNormalmapShaderProgram.h
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
#include "light/DirectionalLight.h"
#include "light/PointLight.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class ModelNormalmapShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& transformComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::TransformComponent>(t_entity);

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
            const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

            SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));
            SetUniform("mvpMatrix", mvp);
            SetUniform("plane", t_scene.GetCurrentClipPlane());

            SetUniform("directionalLightDirection", t_scene.GetDirectionalLight().direction);
            SetUniform("pointLightPosition", t_scene.GetPointLights()[0]->position);
            SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

            SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());
            SetUniform("directionalLight", t_scene.GetDirectionalLight());
            SetUniform("pointLight", *t_scene.GetPointLights()[0]);

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


            SetUniform("normalMap", 2);
            TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKn, GL_TEXTURE2);

            SetUniform("shininess", t_currentMesh.GetDefaultMaterial()->ns);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "model_normalmap";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
