// This file is part of the SgOgl package.
// 
// Filename: LookAtCamera.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include "LookAtCamera.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::LookAtCamera::LookAtCamera()
{
    SG_OGL_CORE_LOG_DEBUG("[LookAtCamera::LookAtCamera()] Create LookAtCamera.");

    Update();
}

sg::ogl::camera::LookAtCamera::LookAtCamera(const glm::vec3 t_position, const float t_yaw, const float t_pitch)
    : m_position{ t_position }
    , m_yaw{ t_yaw }
    , m_pitch{ t_pitch }
{
    SG_OGL_CORE_LOG_DEBUG("[LookAtCamera::LookAtCamera()] Create LookAtCamera.");

    Update();
}

sg::ogl::camera::LookAtCamera::~LookAtCamera() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[LookAtCamera::~LookAtCamera()] Destruct LookAtCamera.");
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::camera::LookAtCamera::SetCameraSpeed(const double t_speed)
{
    m_movementSpeed = t_speed;
}

void sg::ogl::camera::LookAtCamera::SetMouseSensitivity(const float t_sensitivity)
{
    m_mouseSensitivity = t_sensitivity;
}

void sg::ogl::camera::LookAtCamera::SetPosition(const glm::vec3& t_position)
{
    m_position = t_position;
}

void sg::ogl::camera::LookAtCamera::InvertPitch()
{
    m_pitch = -m_pitch;

    // Update Front, Right and Up Vectors using the updated Euler angles.
    Update();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

glm::vec3& sg::ogl::camera::LookAtCamera::GetPosition()
{
    return m_position;
}

const glm::vec3& sg::ogl::camera::LookAtCamera::GetPosition() const
{
    return m_position;
}

glm::mat4 sg::ogl::camera::LookAtCamera::GetViewMatrix() const
{
    return lookAt(m_position, m_position + m_front, m_up);
}

float sg::ogl::camera::LookAtCamera::GetYaw() const
{
    return m_yaw;
}

float sg::ogl::camera::LookAtCamera::GetPitch() const
{
    return m_pitch;
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::camera::LookAtCamera::Update()
{
    // Calculate the new Front vector.
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = normalize(front);

    // Also re-calculate the Right and Up vector.
    m_right = normalize(cross(m_front, m_worldUp));
    m_up = normalize(cross(m_right, m_front));
}

//-------------------------------------------------
// Keyboard && Mouse
//-------------------------------------------------

void sg::ogl::camera::LookAtCamera::ProcessKeyboard(const CameraMovement t_direction, const double t_dt)
{
    const auto velocity{ static_cast<float>(m_movementSpeed * t_dt) };

    if (t_direction == FORWARD)
    {
        m_position += m_front * velocity;
    }

    if (t_direction == BACKWARD)
    {
        m_position -= m_front * velocity;
    }

    if (t_direction == LEFT)
    {
        m_position -= m_right * velocity;
    }

    if (t_direction == RIGHT)
    {
        m_position += m_right * velocity;
    }

    if (t_direction == UP)
    {
        m_position += m_up * velocity;
    }

    if (t_direction == DOWN)
    {
        m_position -= m_up * velocity;
    }
}

void sg::ogl::camera::LookAtCamera::ProcessMouse(const glm::vec2& t_displVec)
{
    m_yaw += t_displVec.y * m_mouseSensitivity;
    m_pitch -= t_displVec.x * m_mouseSensitivity;

    if (m_yaw > 359.0f)
    {
        m_yaw = 359.0f;
    }
    else if (m_yaw < -359.0f)
    {
        m_yaw = -359.0f;
    }

    /*
    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }
    */

    // Update Front, Right and Up Vectors using the updated Euler angles.
    Update();
}
