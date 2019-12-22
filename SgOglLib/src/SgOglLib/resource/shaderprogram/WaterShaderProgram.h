// This file is part of the SgOgl package.
// 
// Filename: WaterShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "Window.h"
#include "buffer/WaterFbos.h"
#include "camera/Camera.h"
#include "light/DirectionalLight.h"
#include "scene/Scene.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "water/Water.h"
#include "ecs/component/WaterComponent.h"
#include "ecs/component/TransformComponent.h"

namespace sg::ogl::resource::shaderprogram
{
    class WaterShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            // get components
            auto& waterComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::WaterComponent>(t_entity);
            auto& transformComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::TransformComponent>(t_entity);

            // get projection matrix
            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };

            // set model matrix
            SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));

            // set vp matrix
            const auto vp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() };
            SetUniform("vpMatrix", vp);

            // set camera position
            SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

            // set near and far
            SetUniform("near", t_scene.GetApplicationContext()->GetProjectionOptions().nearPlane);
            SetUniform("far", t_scene.GetApplicationContext()->GetProjectionOptions().farPlane);

            // set textures
            SetUniform("reflectionMap", 0);
            SetUniform("refractionMap", 1);
            SetUniform("dudvMap", 2);
            SetUniform("normalMap", 3);
            SetUniform("depthMap", 4);

            // set directional light from the scene
            SetUniform("lightPosition", t_scene.GetDirectionalLight().direction);
            SetUniform("lightColor", t_scene.GetDirectionalLight().diffuseIntensity);

            // set move factor
            SetUniform("moveFactor", waterComponent.water->moveFactor);

            // bind textures
            TextureManager::BindForReading(waterComponent.water->GetWaterFbos().GetReflectionColorTextureId(), GL_TEXTURE0);
            TextureManager::BindForReading(waterComponent.water->GetWaterFbos().GetRefractionColorTextureId(), GL_TEXTURE1);
            TextureManager::BindForReading(waterComponent.water->GetDudvTextureId(), GL_TEXTURE2);
            TextureManager::BindForReading(waterComponent.water->GetNormalTextureId(), GL_TEXTURE3);
            TextureManager::BindForReading(waterComponent.water->GetWaterFbos().GetRefractionDepthTextureId(), GL_TEXTURE4);
        }

        std::string GetFolderName() const override
        {
            return "water";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
