// This file is part of the SgOgl package.
// 
// Filename: TerrainShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "Window.h"
#include "camera/LookAtCamera.h"
#include "scene/Scene.h"
#include "terrain/Terrain.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/TransformComponent.h"
#include "ecs/component/TerrainComponent.h"

namespace sg::ogl::resource::shaderprogram
{
    class TerrainShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            // get components
            auto& terrainComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::TerrainComponent>(t_entity);
            auto& transformComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::TransformComponent>(t_entity);

            // get terrain options
            const auto& terrainOptions{ terrainComponent.terrain->GetTerrainOptions() };

            // calc mvp matrix
            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
            const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

            // set model matrix
            SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));

            // set mvp uniform
            SetUniform("mvpMatrix", mvp);

            // set ambient intensity
            SetUniform("ambientIntensity", t_scene.GetAmbientIntensity());

            // set directional light from the scene
            SetUniform("directionalLight", t_scene.GetDirectionalLight());

            // set point light from the scene
            SetUniform("pointLight", t_scene.GetPointLight());

            // set camera position
            SetUniform("cameraPosition", t_scene.GetCurrentCamera().GetPosition());

            // set and bind textures
            auto counter{ 0 };
            for (const auto& entry : terrainOptions.textureContainer)
            {
                SetUniform(entry.first, counter);
                TextureManager::BindForReading(t_scene.GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
                counter++;
            }

            // set and bind normalmap
            SetUniform("normalmap", counter);
            TextureManager::BindForReading(terrainComponent.terrain->GetNormalmapTextureId(), GL_TEXTURE0 + counter);
            counter++;

            // set and bind splatmap
            SetUniform("splatmap", counter);
            TextureManager::BindForReading(terrainComponent.terrain->GetSplatmapTextureId(), GL_TEXTURE0 + counter);
        }

        std::string GetFolderName() const override
        {
            return "terrain";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
