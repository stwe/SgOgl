// This file is part of the SgOgl package.
// 
// Filename: MousePicker.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::input
{
    class MousePicker
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MousePicker() = delete;

        explicit MousePicker(scene::Scene* t_scene);

        MousePicker(const MousePicker& t_other) = delete;
        MousePicker(MousePicker&& t_other) noexcept = delete;
        MousePicker& operator=(const MousePicker& t_other) = delete;
        MousePicker& operator=(MousePicker&& t_other) noexcept = delete;

        ~MousePicker() = default;

        //-------------------------------------------------
        // Mouse Ray
        //-------------------------------------------------

        /**
         * @brief Takes mouse position on screen and return ray in world coords.
         * @param t_mouseX The x mouse position.
         * @param t_mouseY The y mouse position.
         * @return glm::vec3
         */
        glm::vec3 GetRayFromMouse(float t_mouseX, float t_mouseY) const;

        bool RayAabb
        (
            glm::vec3 t_rayOriginWorld,
            glm::vec3 t_rayDirectionWorld,
            glm::vec3 t_min,
            glm::vec3 t_max
        ) const;

        bool RaySphere
        (
            glm::vec3 t_rayOriginWorld,
            glm::vec3 t_rayDirectionWorld,
            glm::vec3 t_sphereCentreWorld,
            float t_sphereRadius,
            float* t_intersectionDistance
        ) const;

    protected:

    private:
        /**
         * @brief Pointer to the parent Scene object.
         */
        scene::Scene* m_scene{ nullptr };
    };
}
