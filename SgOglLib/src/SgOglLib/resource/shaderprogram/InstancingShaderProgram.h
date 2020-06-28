// This file is part of the SgOgl package.
// 
// Filename: InstancingShaderProgram.h
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
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class InstancingShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(
            const scene::Scene& t_scene,
            const entt::entity t_entity,
            const Mesh& t_currentMesh,
            const std::vector<light::PointLight>& t_pointLights,
            const std::vector<light::DirectionalLight>& t_directionalLights
        ) override
        {
            auto& modelInstancesComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::ModelInstancesComponent>(t_entity) };

            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
            SetUniform("viewMatrix", t_scene.GetCurrentCamera().GetViewMatrix());
            SetUniform("fakeNormals", modelInstancesComponent.fakeNormals);

            if (!t_pointLights.empty())
            {
                SetUniform("numPointLights", static_cast<int32_t>(t_pointLights.size()));
                SetUniform("pointLights", t_pointLights);
            }

            if (!t_directionalLights.empty())
            {
                SetUniform("numDirectionalLights", static_cast<int32_t>(t_directionalLights.size()));
                SetUniform("directionalLights", t_directionalLights);
            }

            SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());
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

            SetUniform("shininess", t_currentMesh.GetDefaultMaterial()->ns);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "instancing";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
