// This file is part of the SgOgl package.
// 
// Filename: PointLight.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>

/**
 * Distance  Constant  Linear   Quadratic
 *    7       1.0      0.7      1.8
 *   13       1.0      0.35     0.44
 *   20       1.0      0.22     0.20
 *   32       1.0      0.14     0.07
 *   50       1.0      0.09     0.032
 *   65       1.0      0.07     0.017
 *  100       1.0      0.045    0.0075
 *  160       1.0      0.027    0.0028
 *  200       1.0      0.022    0.0019
 *  325       1.0      0.014    0.0007
 *  600       1.0      0.007    0.0002
 * 3250       1.0      0.0014   0.000007
 */

namespace sg::ogl::light
{
    struct PointLight
    {
        glm::vec3 position{ glm::vec3(0.0f) };

        glm::vec3 ambientIntensity{ glm::vec3(0.2f) };
        glm::vec3 diffuseIntensity{ glm::vec3(1.0f) };
        glm::vec3 specularIntensity{ glm::vec3(1.0f) };

        // Attenuation default distance = 200
        float constant{ 1.0f };
        float linear{ 0.022f };
        float quadratic{ 0.0019f };
    };
}
