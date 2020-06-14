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

namespace sg::ogl::resource::shaderprogram
{
    class ParticleSystemShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, entt::entity t_entity, void* t_object) override
        {
            auto* p{ static_cast<particle::Particle*>(t_object) };

            const auto life{ p->lifeRemaining / p->lifeTime };
            const auto color{ lerp(p->colorEnd, p->colorBegin, life) };
            SetUniform("color", color);

            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());

            const auto viewMatrix{ t_scene.GetCurrentCamera().GetViewMatrix() };

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

            // todo rotation
            //modelMatrix = glm::rotate(modelMatrix, glm::radians(p->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

            const auto size{ glm::lerp(p->sizeEnd, p->sizeBegin, life) };
            modelMatrix = scale(modelMatrix, glm::vec3(size));

            SetUniform("modelViewMatrix", viewMatrix * modelMatrix);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "particle_system";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
