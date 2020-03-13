// This file is part of the SgOgl package.
// 
// Filename: TextShaderProgram.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Application.h"
#include "Window.h"
#include "resource/ShaderProgram.h"

namespace sg::ogl::resource::shaderprogram
{
    class TextShaderProgram : public ShaderProgram
    {
    public:
        void UpdateUniforms(const scene::Scene& t_scene, const glm::vec3& t_vec3) override
        {
            SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetOrthographicProjectionMatrix());
            SetUniform("textTexture", 0);
            SetUniform("textColor", t_vec3);
        }

        [[nodiscard]] std::string GetFolderName() const override
        {
            return "text";
        }

        [[nodiscard]] bool IsBuiltIn() const override
        {
            return true;
        }

    protected:

    private:

    };
}
