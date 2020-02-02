// This file is part of the SgOgl package.
// 
// Filename: Camera.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "math/Plane.h"

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::camera
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class Camera
    {
    public:
        using FrustumPlaneContainer = std::vector<math::Plane>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Camera() = delete;

        explicit Camera(Application* t_application);
        Camera(Application* t_application, const glm::vec3& t_position, float t_yaw, float t_pitch);

        Camera(const Camera& t_other) = delete;
        Camera(Camera&& t_other) noexcept = delete;
        Camera& operator=(const Camera& t_other) = delete;
        Camera& operator=(Camera&& t_other) noexcept = delete;

        virtual ~Camera() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const glm::vec3& GetPosition() const noexcept;
        [[nodiscard]] glm::vec3& GetPosition() noexcept;
        [[nodiscard]] float GetYaw() const;
        [[nodiscard]] float GetPitch() const;
        [[nodiscard]] float GetCameraVelocity() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPosition(const glm::vec3& t_position);
        void SetYaw(float t_yaw);
        void SetPitch(float t_pitch);
        void SetCameraVelocity(float t_velocity);

        void InvertPitch();

        //-------------------------------------------------
        // Virtual
        //-------------------------------------------------

        [[nodiscard]] virtual glm::mat4 GetViewMatrix() const = 0;

        virtual void Input() = 0;
        virtual void Update(double t_dt) = 0;

        //-------------------------------------------------
        // Frustum planes
        //-------------------------------------------------

        void GetFrustumPlanes(std::vector<glm::vec4>& t_planes) const;
        void UpdateFrustumPlanes();

    protected:
        Application* m_application{ nullptr };

        glm::vec3 m_position{ glm::vec3(0.0f) };

        float m_yaw{ 20.0f };
        float m_pitch{ 0.0f };

        float m_cameraVelocity{ 4.0f };

        glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, 1.0f) };
        glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_right{ glm::vec3(1.0f, 0.0f, 0.0f) };

        // frustum points
        glm::vec3 m_farPts[4];
        glm::vec3 m_nearPts[4];

        // frustum planes
        FrustumPlaneContainer m_planes;
    private:

    };
}
