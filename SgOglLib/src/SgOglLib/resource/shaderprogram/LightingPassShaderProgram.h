// This file is part of the SgOgl package.
// 
// Filename: LightingPassShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "buffer/GBufferFbo.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class LightingPassShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(
            const scene::Scene& t_scene,
            const buffer::GBufferFbo& t_gbufferFbo,
            const std::vector<light::PointLight>& t_pointLights,
            const std::vector<light::DirectionalLight>& t_directionalLights
        ) override
        {
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

            SetUniform("gPosition", 0);
            TextureManager::BindForReading(t_gbufferFbo.GetPositionTextureId(), GL_TEXTURE0);
            SetUniform("gNormal", 1);
            TextureManager::BindForReading(t_gbufferFbo.GetNormalTextureId(), GL_TEXTURE1);
            SetUniform("gAlbedoSpec", 2);
            TextureManager::BindForReading(t_gbufferFbo.GetAlbedoSpecTextureId(), GL_TEXTURE2);


            // todo: SetUniform("shininess", t_material.ns);
            /*
            if (t_scene.GetApplicationContext()->registry.has<Material>(t_entity))
            {
                UpdateMaterial(t_scene.GetApplicationContext()->registry.get<Material>(t_entity));
            }
            else
            {
                UpdateMaterial(*t_currentMesh.GetDefaultMaterial());
            }
            */


            SetUniform("shininess", 0.4f);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "lighting_pass";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
