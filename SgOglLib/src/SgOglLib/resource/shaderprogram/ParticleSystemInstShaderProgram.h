// This file is part of the SgOgl package.
// 
// Filename: ParticleSystemInstShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "resource/ShaderProgram.h"
#include "particle/ParticleSystem.h"

namespace sg::ogl::resource::shaderprogram
{
    class ParticleSystemInstShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, entt::entity t_entity, void* t_object) override
        {
            auto& particleSystemComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::ParticleSystemComponent>(t_entity) };

            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
            SetUniform("textureRows", particleSystemComponent.particleSystem->GetTextureRows());
            SetUniform("particleTexture", 0);
            TextureManager::BindForReading(particleSystemComponent.particleSystem->GetTextureId(), GL_TEXTURE0);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "particle_inst";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:
    };
}
