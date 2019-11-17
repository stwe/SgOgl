// This file is part of the SgOgl package.
// 
// Filename: ParticleShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

class ParticleShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& emitterComponent{ t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::ParticleEmitterComponent>(t_entity) };

        SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
        SetUniform("numberOfRows", static_cast<float>(emitterComponent.particleEmitter->GetNumberOfTextureRows()));
        SetUniform("particleTexture", 0);
        sg::ogl::resource::TextureManager::BindForReading(emitterComponent.particleEmitter->GetTextureId(), GL_TEXTURE0);
    }

    std::string GetFolderName() override
    {
        return "particle_anim";
    }

protected:

private:

};
