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

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::scene
{
    class Scene;
}

namespace sg::ogl::input
{
    class MousePicker
    {
    public:
        static constexpr int RECURSION_COUNT{ 200 };
        static constexpr float RAY_RANGE{ 600.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MousePicker() = delete;

        explicit MousePicker(scene::Scene* t_scene, terrain::Terrain* t_terrain = nullptr);

        MousePicker(const MousePicker& t_other) = delete;
        MousePicker(MousePicker&& t_other) noexcept = delete;
        MousePicker& operator=(const MousePicker& t_other) = delete;
        MousePicker& operator=(MousePicker&& t_other) noexcept = delete;

        ~MousePicker() = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        glm::vec3 GetCurrentRay() const;
        glm::vec3 GetCurrentTerrainPoint() const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();

        //-------------------------------------------------
        // Collision
        //-------------------------------------------------

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

        /**
         * @brief Pointer to a Terrain object.
         */
        terrain::Terrain* m_terrain{ nullptr };

        /**
         * @brief The current mouse ray.
         */
        glm::vec3 m_currentRay{ glm::vec3(0.0f) };

        glm::vec3 m_currentTerrainPoint{ glm::vec3(0.0f) };

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

        //-------------------------------------------------
        // Terrain
        //-------------------------------------------------

        glm::vec3 GetPointOnRay(glm::vec3 t_ray, float t_distance) const;
        glm::vec3 BinarySearch(int t_count, float t_start, float t_end, glm::vec3 t_ray) const;

        bool IntersectionInRange(float t_start, float t_end, glm::vec3 t_ray) const;
        bool IsUnderground(glm::vec3 t_point) const;
    };
}
