// This file is part of the SgOgl package.
// 
// Filename: Plane.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtc/matrix_transform.hpp>
#include "Plane.h"

sg::ogl::math::Plane sg::ogl::math::Plane::FromPoints(const glm::vec3& t_v1, const glm::vec3& t_v2, const glm::vec3& t_v3)
{
    Plane plane;

    const auto e1 = t_v2 - t_v1;
    const auto e2 = t_v3 - t_v1;

    plane.normal = normalize(cross(e1, e2));
    plane.distance = -dot(plane.normal, t_v1);

    return plane;
}
