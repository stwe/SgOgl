// This file is part of the SgOgl package.
// 
// Filename: Camera.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include "Camera.h"
#include "Core.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::Camera::Camera(Application* t_application)
    : m_application{ t_application }
{
    SG_OGL_CORE_ASSERT(m_application, "[Camera::Camera()] Null pointer.")
}

sg::ogl::camera::Camera::Camera(Application* t_application, const glm::vec3& t_position, float t_yaw, float t_pitch)
    : m_application{ t_application }
    , m_position{ t_position }
    , m_yaw{ t_yaw }
    , m_pitch{ t_pitch }
{
    SG_OGL_CORE_ASSERT(m_application, "[Camera::Camera()] Null pointer.")
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const glm::vec3& sg::ogl::camera::Camera::GetPosition() const noexcept
{
    return m_position;
}

glm::vec3& sg::ogl::camera::Camera::GetPosition() noexcept
{
    return m_position;
}

float sg::ogl::camera::Camera::GetYaw() const
{
    return m_yaw;
}

float sg::ogl::camera::Camera::GetPitch() const
{
    return m_pitch;
}

float sg::ogl::camera::Camera::GetCameraVelocity() const
{
    return m_cameraVelocity;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::camera::Camera::SetPosition(const glm::vec3& t_position)
{
    m_position = t_position;
}

void sg::ogl::camera::Camera::SetYaw(const float t_yaw)
{
    m_yaw = t_yaw;
}

void sg::ogl::camera::Camera::SetPitch(const float t_pitch)
{
    m_pitch = t_pitch;
}

void sg::ogl::camera::Camera::SetCameraVelocity(const float t_velocity)
{
    m_cameraVelocity = t_velocity;
}

void sg::ogl::camera::Camera::InvertPitch()
{
    m_pitch = -m_pitch;
}
