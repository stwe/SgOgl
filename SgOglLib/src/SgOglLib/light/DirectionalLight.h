// This file is part of the SgOgl package.
// 
// Filename: DirectionalLight.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>

namespace sg::ogl::light
{
    struct DirectionalLight
    {
        glm::vec3 direction{ glm::vec3(0.0f, -1.0f, 0.0f) };
        glm::vec3 diffuseIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 specularIntensity{ glm::vec3(1.0f, 1.0f, 1.0f) };
    };
}
