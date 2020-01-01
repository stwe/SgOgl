// This file is part of the SgOgl package.
// 
// Filename: FirstPersonCamera.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include "Window.h"
#include "OpenGl.h"
#include "FirstPersonCamera.h"
#include "Log.h"
#include "Core.h"
#include "input/MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::FirstPersonCamera::FirstPersonCamera(Application* t_application)
    : Camera(t_application)
{
    SG_OGL_CORE_ASSERT(m_application, "[FirstPersonCamera::FirstPersonCamera()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[FirstPersonCamera::FirstPersonCamera()] Create FirstPersonCamera.");
}

sg::ogl::camera::FirstPersonCamera::FirstPersonCamera(Application* t_application, const glm::vec3& t_position, float t_yaw, float t_pitch)
    : Camera(t_application, t_position, t_yaw, t_pitch)
{
    SG_OGL_CORE_ASSERT(m_application, "[FirstPersonCamera::FirstPersonCamera()] Null pointer.")
    SG_OGL_CORE_LOG_DEBUG("[FirstPersonCamera::FirstPersonCamera()] Create FirstPersonCamera.");
}

sg::ogl::camera::FirstPersonCamera::~FirstPersonCamera() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[FirstPersonCamera::FirstPersonCamera()] Destruct FirstPersonCamera.");
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::camera::FirstPersonCamera::SetMouseSensitivity(const float t_sensitivity)
{
    m_mouseSensitivity = t_sensitivity;
}

void sg::ogl::camera::FirstPersonCamera::InvertPitch(const double t_dt)
{
    m_pitch = -m_pitch;

    Update(t_dt);
}

//-------------------------------------------------
// Override
//-------------------------------------------------

glm::mat4 sg::ogl::camera::FirstPersonCamera::GetViewMatrix() const
{
    return lookAt(m_position, m_position + m_front, m_up);
}

void sg::ogl::camera::FirstPersonCamera::Input()
{
    // handle mouse input
    ProcessMouse();
}

void sg::ogl::camera::FirstPersonCamera::Update(const double t_dt)
{
    // handle keyboard input
    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_W))
    {
        ProcessKeyboard(CameraMovement::FORWARD, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_S))
    {
        ProcessKeyboard(CameraMovement::BACKWARD, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_A))
    {
        ProcessKeyboard(CameraMovement::LEFT, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_D))
    {
        ProcessKeyboard(CameraMovement::RIGHT, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_O))
    {
        ProcessKeyboard(CameraMovement::UP, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_U))
    {
        ProcessKeyboard(CameraMovement::DOWN, t_dt);
    }

    if (m_application->GetWindow().IsKeyPressed(GLFW_KEY_I))
    {
        SG_OGL_CORE_LOG_INFO("[FirstPersonCamera::Update()] Camera x: {}  y: {}  z: {}", GetPosition().x, GetPosition().y, GetPosition().z);
        SG_OGL_CORE_LOG_INFO("[FirstPersonCamera::Update()] Camera yaw: {}  pitch: {}", GetYaw(), GetPitch());
    }

    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = normalize(front);

    // re-calculate the Right and Up vector.
    m_right = normalize(cross(m_front, m_worldUp));
    m_up = normalize(cross(m_right, m_front));
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::camera::FirstPersonCamera::ProcessKeyboard(const CameraMovement t_direction, const double t_dt)
{
    const auto velocity{ static_cast<float>(m_cameraVelocity * t_dt) };

    if (t_direction == CameraMovement::FORWARD)
    {
        m_position += m_front * velocity;
    }

    if (t_direction == CameraMovement::BACKWARD)
    {
        m_position -= m_front * velocity;
    }

    if (t_direction == CameraMovement::LEFT)
    {
        m_position -= m_right * velocity;
    }

    if (t_direction == CameraMovement::RIGHT)
    {
        m_position += m_right * velocity;
    }

    if (t_direction == CameraMovement::UP)
    {
        m_position += m_up * velocity;
    }

    if (t_direction == CameraMovement::DOWN)
    {
        m_position -= m_up * velocity;
    }
}

void sg::ogl::camera::FirstPersonCamera::ProcessMouse()
{
    if (m_application->GetMouseInput().IsRightButtonPressed())
    {
        m_yaw += m_application->GetMouseInput().GetDisplVec().x * m_mouseSensitivity;
        m_pitch += m_application->GetMouseInput().GetDisplVec().y * m_mouseSensitivity;

        m_yaw = std::clamp(m_yaw, -360.0f, 360.0f);

        // ensures the user will be able to look up to the sky and down to his feet but not further
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }
}