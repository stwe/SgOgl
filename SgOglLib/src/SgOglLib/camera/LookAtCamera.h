#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::camera
{
    /**
     * @brief Possible options for camera movement.
     */
    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class LookAtCamera
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        LookAtCamera()
        {
            Update();
        }

        LookAtCamera(const glm::vec3 t_position, const float t_yaw, const float t_pitch)
            : m_position{ t_position }
            , m_yaw{ t_yaw }
            , m_pitch{ t_pitch }
        {
            Update();
        }

        LookAtCamera(const LookAtCamera& t_other) = delete;
        LookAtCamera(LookAtCamera&& t_other) noexcept = delete;
        LookAtCamera& operator=(const LookAtCamera& t_other) = delete;
        LookAtCamera& operator=(LookAtCamera&& t_other) noexcept = delete;

        ~LookAtCamera() noexcept = default;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCameraSpeed(const float t_speed) { m_movementSpeed = t_speed; }
        void SetMouseSensitivity(const float t_sensitivity) { m_mouseSensitivity = t_sensitivity; }
        void SetPosition(const glm::vec3& t_position) { m_position = t_position; }

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        auto& GetPosition() { return m_position; }
        const auto& GetPosition() const { return m_position; }
        auto GetViewMatrix() const { return lookAt(m_position, m_position + m_front, m_up); }

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        /**
         * @brief Calculates the front vector from the Camera's (updated) Euler Angles.
         */
        void Update()
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

        void ProcessKeyboard(const camera::CameraMovement t_direction, const float t_dt)
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

        void ProcessMouse(const glm::vec2& t_displVec)
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

    protected:

    private:
        float m_movementSpeed{ 1.0f };
        float m_mouseSensitivity{ 0.2f };

        glm::vec3 m_position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, 1.0f) };
        glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };
        glm::vec3 m_right{ glm::vec3(1.0f, 0.0f, 0.0f) };

        float m_yaw{ 90.0f };
        float m_pitch{ 0.0f };
    };
}
