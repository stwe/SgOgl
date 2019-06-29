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

        LookAtCamera();

        LookAtCamera(glm::vec3 t_position, float t_yaw, float t_pitch);

        LookAtCamera(const LookAtCamera& t_other) = delete;
        LookAtCamera(LookAtCamera&& t_other) noexcept = delete;
        LookAtCamera& operator=(const LookAtCamera& t_other) = delete;
        LookAtCamera& operator=(LookAtCamera&& t_other) noexcept = delete;

        ~LookAtCamera() noexcept = default;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCameraSpeed(float t_speed);
        void SetMouseSensitivity(float t_sensitivity);
        void SetPosition(const glm::vec3& t_position);

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        glm::vec3& GetPosition();
        const glm::vec3& GetPosition() const;
        glm::mat4 GetViewMatrix() const;

        //-------------------------------------------------
        // Update
        //-------------------------------------------------

        /**
         * @brief Calculates the front vector from the Camera's (updated) Euler Angles.
         */
        void Update();

        //-------------------------------------------------
        // Keyboard && Mouse
        //-------------------------------------------------

        void ProcessKeyboard(CameraMovement t_direction, float t_dt);
        void ProcessMouse(const glm::vec2& t_displVec);

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