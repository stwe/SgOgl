// This file is part of the SgOgl package.
// 
// Filename: GuiShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "OpenGl.h"
#include "Application.h"
#include "scene/Scene.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class GuiShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& transformComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::TransformComponent>(t_entity) };
            SetUniform("transformationMatrix", static_cast<glm::mat4>(transformComponent));

            auto& guiComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::GuiComponent>(t_entity) };
            SetUniform("guiTexture", 0);
            TextureManager::BindForReading(guiComponent.textureId, GL_TEXTURE0);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "gui";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
