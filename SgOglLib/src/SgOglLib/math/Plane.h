// This file is part of the SgOgl package.
// 
// Filename: Plane.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>

namespace sg::ogl::math
{
    class Plane
    {
    public:
        glm::vec3 normal{ glm::vec3(0.0f, 1.0f, 0.0f) };
        float distance{ 0.0f };

        static Plane FromPoints(const glm::vec3& t_v1, const glm::vec3& t_v2, const glm::vec3& t_v3);

    protected:

    private:

    };
}
