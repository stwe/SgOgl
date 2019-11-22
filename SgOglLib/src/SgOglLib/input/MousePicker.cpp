// This file is part of the SgOgl package.
// 
// Filename: MousePicker.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtc/matrix_transform.hpp>
#include "MousePicker.h"
#include "Core.h"
#include "Log.h"
#include "Application.h"
#include "Window.h"
#include "camera/LookAtCamera.h"
#include "scene/Scene.h"

sg::ogl::input::MousePicker::MousePicker(scene::Scene* t_scene)
    : m_scene{ t_scene }
{
    SG_OGL_CORE_ASSERT(m_scene, "[MousePicker::MousePicker()] Null pointer.")
}

glm::vec3 sg::ogl::input::MousePicker::GetRayFromMouse(const float t_mouseX, const float t_mouseY) const
{
    const auto& projectionOptions{ m_scene->GetApplicationContext()->GetProjectionOptions() };
    const auto& projectionMatrix{ m_scene->GetApplicationContext()->GetWindow().GetProjectionMatrix() };
    const auto viewMatrix{ m_scene->GetCurrentCamera().GetViewMatrix() };

    // screen space (viewport coordinates)
    const auto x{ (2.0f * t_mouseX) / static_cast<float>(projectionOptions.width) - 1.0f };
    const auto y{ 1.0f - (2.0f * t_mouseY) / static_cast<float>(projectionOptions.height) };
    const auto z{ 1.0f };

    // normalised device space
    const auto rayNds{ glm::vec3(x, y, z) };

    // clip space
    const auto rayClip{ glm::vec4(rayNds.x, rayNds.y, -1.0, 1.0) };

    // eye space
    auto rayEye{ inverse(projectionMatrix) * rayClip };
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    // world space
    auto rayWorld{ glm::vec3(inverse(viewMatrix) * rayEye) };

    // normalise the vector
    rayWorld = glm::normalize(rayWorld);

    return rayWorld;
}

bool sg::ogl::input::MousePicker::RayAabb(
    const glm::vec3 t_rayOriginWorld,
    const glm::vec3 t_rayDirectionWorld,
    const glm::vec3 t_min,
    const glm::vec3 t_max
) const
{
    auto tmin{ (t_min.x - t_rayOriginWorld.x) / t_rayDirectionWorld.x };
    auto tmax{ (t_max.x - t_rayOriginWorld.x) / t_rayDirectionWorld.x };

    if (tmin > tmax)
    {
        std::swap(tmin, tmax);
    }

    auto tymin{ (t_min.y - t_rayOriginWorld.y) / t_rayDirectionWorld.y };
    auto tymax{ (t_max.y - t_rayOriginWorld.y) / t_rayDirectionWorld.y };

    if (tymin > tymax)
    {
        std::swap(tymin, tymax);
    }

    if (tmin > tymax || tymin > tmax)
    {
        return false;
    }

    if (tymin > tmin)
    {
        tmin = tymin;
    }

    if (tymax < tmax)
    {
        tmax = tymax;
    }

    auto tzmin{ (t_min.z - t_rayOriginWorld.z) / t_rayDirectionWorld.z };
    auto tzmax{ (t_max.z - t_rayOriginWorld.z) / t_rayDirectionWorld.z };

    if (tzmin > tzmax)
    {
        std::swap(tzmin, tzmax);
    }

    if (tmin > tzmax || tzmin > tmax)
    {
        return false;
    }

    if (tzmin > tmin)
    {
        tmin = tzmin;
    }

    if (tzmax < tmax)
    {
        tmax = tzmax;
    }

    return true;
}

bool sg::ogl::input::MousePicker::RaySphere(
    const glm::vec3 t_rayOriginWorld,
    const glm::vec3 t_rayDirectionWorld,
    const glm::vec3 t_sphereCentreWorld,
    const float t_sphereRadius,
    float* t_intersectionDistance
) const
{
    // work out components of quadratic
    const auto distToSphere{ t_rayOriginWorld - t_sphereCentreWorld };
    const auto b{ dot(t_rayDirectionWorld, distToSphere) };
    const auto c{ dot(distToSphere, distToSphere) - t_sphereRadius * t_sphereRadius };
    const auto bSquaredMinusC{ b * b - c };

    // check for "imaginary" answer == ray completely misses sphere
    if (bSquaredMinusC < 0.0f)
    {
        return false;
    }

    // check for ray hitting twice (in and out of the sphere)
    if (bSquaredMinusC > 0.0f)
    {
        // get the 2 intersection distances along ray
        const auto tA{ -b + sqrt(bSquaredMinusC) };
        const auto tB{ -b - sqrt(bSquaredMinusC) };
        *t_intersectionDistance = tB;

        // if behind viewer, throw one or both away
        if (tA < 0.0)
        {
            if (tB < 0.0)
            {
                return false;
            }
        }
        else if (tB < 0.0)
        {
            *t_intersectionDistance = tA;
        }

        return true;
    }

    // check for ray hitting once (skimming the surface)
    if (0.0f == bSquaredMinusC)
    {
        // if behind viewer, throw away
        const auto t{ -b + sqrt(bSquaredMinusC) };

        if (t < 0.0f)
        {
            return false;
        }

        *t_intersectionDistance = t;
        return true;
    }

    // note: could also check if ray origin is inside sphere radius
    return false;
}
