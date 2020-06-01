// This file is part of the SgOgl package.
// 
// Filename: FirstPersonCamera.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Camera.h"

namespace sg::ogl::camera
{
    class FirstPersonCamera : public Camera
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        FirstPersonCamera() = delete;

        FirstPersonCamera(const std::string& t_name, Application* t_application);
        FirstPersonCamera(const std::string& t_name, Application* t_application, const glm::vec3& t_position, float t_yaw, float t_pitch);

        FirstPersonCamera(const FirstPersonCamera& t_other) = delete;
        FirstPersonCamera(FirstPersonCamera&& t_other) noexcept = delete;
        FirstPersonCamera& operator=(const FirstPersonCamera& t_other) = delete;
        FirstPersonCamera& operator=(FirstPersonCamera&& t_other) noexcept = delete;

        ~FirstPersonCamera() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetMouseSensitivity(float t_sensitivity);

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        [[nodiscard]] glm::mat4 GetViewMatrix() const override;

        void Input() override;
        void Update(double t_dt) override;

    protected:

    private:
        float m_mouseSensitivity{ 0.1f };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void ProcessKeyboard(CameraMovement t_direction, double t_dt);
        void ProcessMouse();
    };
}
