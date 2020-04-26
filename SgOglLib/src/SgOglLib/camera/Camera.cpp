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

//-------------------------------------------------
// Frustum planes
//-------------------------------------------------

void sg::ogl::camera::Camera::GetFrustumPlanes(std::vector<glm::vec4>& t_planes) const
{
    for (auto i{ 0u }; i < m_planes.size(); ++i)
    {
        t_planes[i].x = m_planes[i].normal.x;
        t_planes[i].y = m_planes[i].normal.y;
        t_planes[i].z = m_planes[i].normal.z;
        t_planes[i].w = m_planes[i].distance;
    }
}

void sg::ogl::camera::Camera::UpdateFrustumPlanes()
{
    if (m_planes.empty())
    {
        m_planes.resize(6);
    }

    const auto& projectionOptions{ m_application->GetProjectionOptions() };

    const auto cN{ m_position + m_front * projectionOptions.nearPlane };
    const auto cF{ m_position + m_front * projectionOptions.farPlane };

    const auto aspectRatio{ static_cast<float>(projectionOptions.width) / static_cast<float>(projectionOptions.height) };

    const auto hnear{ 2.0f * glm::tan(glm::radians(projectionOptions.fovDeg * 0.5f)) * projectionOptions.nearPlane };
    const auto wnear{ hnear * aspectRatio };
    const auto hfar{ 2.0f * glm::tan(glm::radians(projectionOptions.fovDeg * 0.5f)) * projectionOptions.farPlane };
    const auto wfar{ hfar * aspectRatio };
    const auto hHnear{ hnear * 0.5f };
    const auto hWnear{ wnear * 0.5f };
    const auto hHfar{ hfar * 0.5f };
    const auto hWfar{ wfar * 0.5f };

    m_farPts[0] = cF + m_up * hHfar - m_right * hWfar;
    m_farPts[1] = cF - m_up * hHfar - m_right * hWfar;
    m_farPts[2] = cF - m_up * hHfar + m_right * hWfar;
    m_farPts[3] = cF + m_up * hHfar + m_right * hWfar;

    m_nearPts[0] = cN + m_up * hHnear - m_right * hWnear;
    m_nearPts[1] = cN - m_up * hHnear - m_right * hWnear;
    m_nearPts[2] = cN - m_up * hHnear + m_right * hWnear;
    m_nearPts[3] = cN + m_up * hHnear + m_right * hWnear;

    m_planes[0] = math::Plane::FromPoints(m_nearPts[3], m_nearPts[0], m_farPts[0]);
    m_planes[1] = math::Plane::FromPoints(m_nearPts[1], m_nearPts[2], m_farPts[2]);
    m_planes[2] = math::Plane::FromPoints(m_nearPts[0], m_nearPts[1], m_farPts[1]);
    m_planes[3] = math::Plane::FromPoints(m_nearPts[2], m_nearPts[3], m_farPts[2]);
    m_planes[4] = math::Plane::FromPoints(m_nearPts[0], m_nearPts[3], m_nearPts[2]);
    m_planes[5] = math::Plane::FromPoints(m_farPts[3], m_farPts[0], m_farPts[1]);
}
