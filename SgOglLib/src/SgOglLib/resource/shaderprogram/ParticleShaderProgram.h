// This file is part of the SgOgl package.
// 
// Filename: ParticleShaderProgram.h
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
#include "particle/ParticleEmitter.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "ecs/component/Components.h"

namespace sg::ogl::resource::shaderprogram
{
    class ParticleShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const entt::entity t_entity, const Mesh& t_currentMesh) override
        {
            auto& emitterComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::ParticleEmitterComponent>(t_entity) };

            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
            SetUniform("numberOfRows", static_cast<float>(emitterComponent.particleEmitter->GetNumberOfTextureRows()));
            SetUniform("particleTexture", 0);
            TextureManager::BindForReading(emitterComponent.particleEmitter->GetTextureId(), GL_TEXTURE0);
        }

        std::string GetFolderName() const override
        {
            return "particle_anim";
        }

        bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
