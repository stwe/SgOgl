// This file is part of the SgOgl package.
// 
// Filename: DomeShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Application.h"
#include "Window.h"
#include "scene/Scene.h"
#include "camera/Camera.h"
#include "resource/ShaderProgram.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class DomeShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& transformComponent = t_scene.GetApplicationContext()->registry.get<ecs::component::TransformComponent>(t_entity);

            const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
            const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

            SetUniform("mvpMatrix", mvp);
            SetUniform("worldMatrix", static_cast<glm::mat4>(transformComponent));
        }

        std::string GetFolderName() const override
        {
            return "dome";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
