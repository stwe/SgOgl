// This file is part of the SgOgl package.
// 
// Filename: TerrainQuadtreeShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include "Window.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/ShaderProgram.h"
#include "terrain/TerrainQuadtree.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class TerrainQuadtreeShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& terrainQuadtreeComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::TerrainQuadtreeComponent>(t_entity) };

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };

            const auto position{ terrainQuadtreeComponent.terrainQuadtree->localPosition };
            const auto scale{ terrainQuadtreeComponent.terrainQuadtree->localScale };

            auto localMatrix{ glm::mat4(1.0f) };
            localMatrix = translate(localMatrix, glm::vec3(position.x, 0.0f, position.y));
            localMatrix = glm::scale(localMatrix, glm::vec3(scale.x, 0.0f, scale.y));

            auto worldMatrix{ glm::mat4(1.0f) };
            worldMatrix = translate(worldMatrix, glm::vec3(-5.0f, 0.0f, -5.0f));
            worldMatrix = glm::scale(worldMatrix, glm::vec3(10.0f, 1.0f, 10.0f));

            SetUniform("localMatrix", localMatrix);
            SetUniform("worldMatrix", worldMatrix);
            SetUniform("viewProjectionMatrix", projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix());
        }

        [[nodiscard]] Options GetOptions() const override
        {
            return VERTEX_SHADER | TESSELLATION_CONTROL_SHADER |TESSELLATION_EVALUATION_SHADER | GEOMETRY_SHADER | FRAGMENT_SHADER;
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "terrain_quadtree";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
