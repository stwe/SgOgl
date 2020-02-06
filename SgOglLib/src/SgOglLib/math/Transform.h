// This file is part of the SgOgl package.
// 
// Filename: Transform.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::math
{
    struct Transform
    {
        glm::vec3 position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 rotation{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale{ glm::vec3(1.0f, 1.0f, 1.0f) };

        explicit operator glm::mat4() const
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = translate(modelMatrix, position);
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, scale);

            return modelMatrix;
        }
    };
}
