// This file is part of the SgOgl package.
// 
// Filename: ParticleSystemShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/gtx/compatibility.hpp>
#include "resource/ShaderProgram.h"
#include "particle/Particle.h"
#include "particle/ParticleSystem.h"

namespace sg::ogl::resource::shaderprogram
{
    class ParticleSystemShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, entt::entity t_entity, void* t_object) override
        {
            auto& particleSystemComponent{ t_scene.GetApplicationContext()->registry.get<ecs::component::ParticleSystemComponent>(t_entity) };
            SetUniform("particleTexture", 0);
            TextureManager::BindForReading(particleSystemComponent.particleSystem->GetTextureId(), GL_TEXTURE0);

            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());

            const auto viewMatrix{ t_scene.GetCurrentCamera().GetViewMatrix() };

            auto* p{ static_cast<particle::Particle*>(t_object) };

            auto modelMatrix = translate(glm::mat4(1.0f), p->position);
            modelMatrix[0][0] = viewMatrix[0][0];
            modelMatrix[0][1] = viewMatrix[1][0];
            modelMatrix[0][2] = viewMatrix[2][0];
            modelMatrix[1][0] = viewMatrix[0][1];
            modelMatrix[1][1] = viewMatrix[1][1];
            modelMatrix[1][2] = viewMatrix[2][1];
            modelMatrix[2][0] = viewMatrix[0][2];
            modelMatrix[2][1] = viewMatrix[1][2];
            modelMatrix[2][2] = viewMatrix[2][2];

            modelMatrix = rotate(modelMatrix, glm::radians(p->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = scale(modelMatrix, glm::vec3(p->scale));

            SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "particle";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
