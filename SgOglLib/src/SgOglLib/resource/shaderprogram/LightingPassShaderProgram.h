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
#include "resource/Mesh.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

namespace sg::ogl::resource::shaderprogram
{
    class LightingPassShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const Mesh& t_mesh, const buffer::GBufferFbo& t_gbufferFbo) override
        {
            SetUniform("numLights", static_cast<int32_t>(t_scene.GetPointLights().size()));

            SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());
            SetUniform("directionalLight", t_scene.GetDirectionalLight());

            SetUniform("pointLights", t_scene.GetPointLights());

            SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

            SetUniform("gPosition", 0);
            TextureManager::BindForReading(t_gbufferFbo.GetPositionTextureId(), GL_TEXTURE0);
            SetUniform("gNormal", 1);
            TextureManager::BindForReading(t_gbufferFbo.GetNormalTextureId(), GL_TEXTURE1);
            SetUniform("gAlbedoSpec", 2);
            TextureManager::BindForReading(t_gbufferFbo.GetAlbedoSpecTextureId(), GL_TEXTURE2);
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
