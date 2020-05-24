// This file is part of the SgOgl package.
// 
// Filename: GBufferPassShaderProgram.h
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
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class GBufferPassShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& transformComponent{ t_scene.GetApplicationContext()->registry.get<math::Transform>(t_entity) };

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
            const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

            SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));
            SetUniform("plane", t_scene.GetCurrentClipPlane());
            SetUniform("mvpMatrix", mvp);

            if (t_scene.GetApplicationContext()->registry.has<Material>(t_entity))
            {
                UpdateMaterial(t_scene.GetApplicationContext()->registry.get<Material>(t_entity));
            }
            else
            {
                UpdateMaterial(*t_currentMesh.GetDefaultMaterial());
            }
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "gbuffer_pass";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:
        void UpdateMaterial(const Material& t_material)
        {
            SetUniform("diffuseColor", t_material.kd);
            SetUniform("hasDiffuseMap", t_material.HasDiffuseMap());
            if (t_material.HasDiffuseMap())
            {
                SetUniform("diffuseMap", 0);
                TextureManager::BindForReading(t_material.mapKd, GL_TEXTURE0);
            }

            SetUniform("specularColor", t_material.ks);
            SetUniform("hasSpecularMap", t_material.HasSpecularMap());
            if (t_material.HasSpecularMap())
            {
                SetUniform("specularMap", 1);
                TextureManager::BindForReading(t_material.mapKs, GL_TEXTURE1);
            }

            SetUniform("hasNormalMap", t_material.HasNormalMap());
            if (t_material.HasNormalMap())
            {
                SetUniform("normalMap", 2);
                TextureManager::BindForReading(t_material.mapKn, GL_TEXTURE2);
            }
        }
    };
}
