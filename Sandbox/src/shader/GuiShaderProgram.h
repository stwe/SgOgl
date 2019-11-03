// This file is part of the SgOgl package.
// 
// Filename: GuiShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "resource/ShaderProgram.h"

class GuiShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& transformComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TransformComponent>(t_entity);
        SetUniform("transformationMatrix", static_cast<glm::mat4>(transformComponent));

        auto& guiComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::GuiComponent>(t_entity);
        SetUniform("guiTexture", 0);
        sg::ogl::resource::TextureManager::BindForReading(guiComponent.textureId, GL_TEXTURE0);
    }

    std::string GetFolderName() override
    {
        return "gui";
    }

protected:

private:

};
