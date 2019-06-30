#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Core.h"

namespace sg::ogl::math
{
    struct SG_OGL_API Transform
    {
        auto GetModelMatrix() const
        {
            const auto translationMatrix = translate(glm::mat4(1.0f), position);

            const auto rotationXMatrix = rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            const auto rotationYMatrix = rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            const auto rotationZMatrix = rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            const auto rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

            const auto scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

            return translationMatrix * rotationMatrix * scaleMatrix;
        }

        glm::vec3 position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 rotation{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale{ glm::vec3(1.0f, 1.0f, 1.0f) };
    };
}
