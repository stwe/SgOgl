#include "LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::LookAtCamera::LookAtCamera()
{
    Update();
}

sg::ogl::camera::LookAtCamera::LookAtCamera(const glm::vec3 t_position, const float t_yaw, const float t_pitch)
    : m_position{ t_position }
    , m_yaw{ t_yaw }
    , m_pitch{ t_pitch }
{
    Update();
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::camera::LookAtCamera::SetCameraSpeed(const float t_speed)
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

void sg::ogl::camera::LookAtCamera::ProcessKeyboard(const CameraMovement t_direction, const float t_dt)
{
    const auto velocity{ m_movementSpeed * t_dt };

    if (t_direction == FORWARD)
        m_position += m_front * velocity;
    if (t_direction == BACKWARD)
        m_position -= m_front * velocity;
    if (t_direction == LEFT)
        m_position -= m_right * velocity;
    if (t_direction == RIGHT)
        m_position += m_right * velocity;
    if (t_direction == UP)
        m_position += m_up * velocity;
    if (t_direction == DOWN)
        m_position -= m_up * velocity;
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

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    /*
    if (m_pitch < 0.0f)
    {
        m_pitch = 0.0f;
    }
    else if (m_pitch > 90.0f)
    {
        m_pitch = 90.0f;
    }
    */

    // Update Front, Right and Up Vectors using the updated Euler angles.
    Update();
}