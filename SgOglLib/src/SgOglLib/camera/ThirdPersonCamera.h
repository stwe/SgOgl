// This file is part of the SgOgl package.
// 
// Filename: ThirdPersonCamera.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "Camera.h"

namespace sg::ogl::camera
{
    class ThirdPersonCamera : public Camera
    {
    public:
        static constexpr auto SPEED{ 0.7f };
        static constexpr auto DEFAULT_DISTANCE{ 20.0f };
        static constexpr auto MIN_DISTANCE{ 5.0f };
        static constexpr auto MAX_DISTANCE{ 100.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ThirdPersonCamera() = delete;

        ThirdPersonCamera(Application* t_application, glm::vec3& t_playerPosition);

        ThirdPersonCamera(const ThirdPersonCamera& t_other) = delete;
        ThirdPersonCamera(ThirdPersonCamera&& t_other) noexcept = delete;
        ThirdPersonCamera& operator=(const ThirdPersonCamera& t_other) = delete;
        ThirdPersonCamera& operator=(ThirdPersonCamera&& t_other) noexcept = delete;

        ~ThirdPersonCamera() noexcept;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetPlayerPosition(const glm::vec3& t_playerPosition);

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Input() override;
        void Update(double t_dt) override;

    protected:

    private:
        glm::vec3 m_playerPosition{ glm::vec3(0.0f) };

        glm::vec3 m_rotation{ glm::vec3(0.0f) }; // todo

        float m_distanceFromPlayer{ DEFAULT_DISTANCE };
        float m_angleAroundPlayer{ 0.0f };

        //-------------------------------------------------
        // Input
        //-------------------------------------------------

        void ProcessMouse();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void CalculateZoom();
        void CalculatePitch();
        void CalculateAngleAroundPlayer();

        [[nodiscard]] float CalculateHorizontalDistance() const;
        [[nodiscard]] float CalculateVerticalDistance() const;

        void CalculateCameraPosition(float t_hDist, float t_vDist);
    };
}
