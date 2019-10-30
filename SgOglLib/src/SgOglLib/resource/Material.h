// This file is part of the SgOgl package.
// 
// Filename: Material.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include <glm/vec3.hpp>
#include "Core.h"

namespace sg::ogl::resource
{
    struct SG_OGL_API Material
    {
        /**
         * @brief The material name statement.
         */
        std::string newmtl;

        /**
         * @brief The ambient color of the material.
         */
        glm::vec3 ka{ glm::vec3(0.2f, 0.2f, 0.2f) };

        /**
         * @brief The diffuse color of the material.
         */
        glm::vec3 kd{ glm::vec3(0.8f, 0.8f, 0.8f) };

        /**
         * @brief The specular color of the material.
         */
        glm::vec3 ks{ glm::vec3(1.0f, 1.0f, 1.0f) };

        /**
         * @brief Can be a number from 0 to 10 which represents
         *        various material lighting and shading effects.
         *        illum = 1 indicates a flat material with no specular highlights, so the value of Ks is not used.
         *        illum = 2 denotes the presence of specular highlights, and so a specification for Ks is required.
         */
        int illum{ 2 };

        /**
         * @brief Specifies the specular exponent (shininess) for the material.
         *        Ns values normally range from 0 to 1000.
         */
        float ns{ 0.0f };

        /**
         * @brief The transparency of the material.
         *        A factor of 1.0 is fully opaque.
         *        A factor of 0.0 is fully dissolved (completely transparent).
         */
        float d{ 1.0f };

        /**
         * @brief The ambient texture map id.
         */
        uint32_t mapKa{ 0 };

        /**
         * @brief The diffuse texture map id. Most of time, it will be the same as
         *        ambient texture map.
         */
        uint32_t mapKd{ 0 };

        /**
         * @brief The specular texture map id.
         */
        uint32_t mapKs{ 0 };

        /**
         * @brief The normal texture map id.
         */
        uint32_t mapBump{ 0 };

        bool HasAmbientMap() const;
        bool HasDiffuseMap() const;
        bool HasSpecularMap() const;
        bool HasNormalMap() const;
    };
}
