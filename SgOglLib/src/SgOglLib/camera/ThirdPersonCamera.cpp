// This file is part of the SgOgl package.
// 
// Filename: ThirdPersonCamera.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtc/matrix_transform.hpp>
#include "ThirdPersonCamera.h"
#include "Application.h"
#include "Window.h"
#include "OpenGl.h"
#include "Core.h"
#include "input/MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::ThirdPersonCamera::ThirdPersonCamera(const std::string& t_name, Application* t_application, const glm::vec3& t_playerPosition)
    : Camera(t_name, t_application)
    , m_playerPosition{ t_playerPosition }
{
    SG_OGL_CORE_ASSERT(m_application, "[ThirdPersonCamera::ThirdPersonCamera()] Null pointer.");

    Log::SG_OGL_CORE_LOG_DEBUG("[ThirdPersonCamera::ThirdPersonCamera()] Create ThirdPersonCamera {}.", m_name);
}

sg::ogl::camera::ThirdPersonCamera::~ThirdPersonCamera() noexcept
{
    Log::SG_OGL_CORE_LOG_DEBUG("[ThirdPersonCamera::~ThirdPersonCamera()] Destruct ThirdPersonCamera {}.", m_name);
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

float sg::ogl::camera::ThirdPersonCamera::GetPlayerRotationY() const
{
    return m_playerRotY;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::camera::ThirdPersonCamera::SetPlayerPosition(const glm::vec3& t_playerPosition)
{
    m_playerPosition = t_playerPosition;
}

void sg::ogl::camera::ThirdPersonCamera::SetPlayerRotationY(const float t_yRotation)
{
    m_playerRotY = t_yRotation;
}

//-------------------------------------------------
// Override
//-------------------------------------------------

glm::mat4 sg::ogl::camera::ThirdPersonCamera::GetViewMatrix() const
{
    return lookAt(m_position, m_playerPosition, m_worldUp);
}

void sg::ogl::camera::ThirdPersonCamera::Input()
{
}

void sg::ogl::camera::ThirdPersonCamera::Update(const double t_dt)
{
    ProcessMouse();
    CalculateCameraPosition(CalculateHorizontalDistance(), CalculateVerticalDistance());

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_I))
    {
        Log::SG_OGL_CORE_LOG_INFO("[ThirdPersonCamera::Update()] Camera x: {}  y: {}  z: {}", GetPosition().x, GetPosition().y, GetPosition().z);
        Log::SG_OGL_CORE_LOG_INFO("[ThirdPersonCamera::Update()] Camera yaw: {}  pitch: {}", GetYaw(), GetPitch());
    }
}

//-------------------------------------------------
// Input
//-------------------------------------------------

void sg::ogl::camera::ThirdPersonCamera::ProcessMouse()
{
    // mouse wheel scrolled - distance from the player
    CalculateZoom();

    // right mouse button - pitch
    CalculatePitch();

    // left mouse button - around the player
    CalculateAngleAroundPlayer();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::camera::ThirdPersonCamera::CalculateZoom()
{
    if (m_application->GetMouseInput().IsScrolled())
    {
        const auto zoomLevel{ m_application->GetMouseInput().GetScrollOffset().y * SPEED };
        m_distanceFromPlayer -= zoomLevel;
        m_distanceFromPlayer = std::clamp(m_distanceFromPlayer, MIN_DISTANCE, MAX_DISTANCE);

        m_application->GetMouseInput().SetScrolled(false);
    }
}

void sg::ogl::camera::ThirdPersonCamera::CalculatePitch()
{
    if (input::MouseInput::IsRightButtonPressed())
    {
        const auto pitchChange{ m_application->GetMouseInput().GetDisplVec().y * SPEED };
        m_pitch -= pitchChange;
    }

    m_pitch = std::clamp(m_pitch, 1.0f, 89.0f);
}

void sg::ogl::camera::ThirdPersonCamera::CalculateAngleAroundPlayer()
{
    if (input::MouseInput::IsLeftButtonPressed())
    {
        const auto angleChange{ m_application->GetMouseInput().GetDisplVec().x * SPEED };
        m_angleAroundPlayer -= angleChange;
    }
}

float sg::ogl::camera::ThirdPersonCamera::CalculateHorizontalDistance() const
{
    return m_distanceFromPlayer * glm::cos(glm::radians(m_pitch));
}

float sg::ogl::camera::ThirdPersonCamera::CalculateVerticalDistance() const
{
    return m_distanceFromPlayer * glm::sin(glm::radians(m_pitch));
}

void sg::ogl::camera::ThirdPersonCamera::CalculateCameraPosition(const float t_hDist, const float t_vDist)
{
    const auto theta{ m_playerRotY + m_angleAroundPlayer };

    const auto offsetX{ t_hDist * glm::sin(glm::radians(theta)) };
    const auto offsetZ{ t_hDist * glm::cos(glm::radians(theta)) };

    m_position.x = m_playerPosition.x - offsetX;
    m_position.z = m_playerPosition.z - offsetZ;
    m_position.y = m_playerPosition.y + t_vDist;
}
