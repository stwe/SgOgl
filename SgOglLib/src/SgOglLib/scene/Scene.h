#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Core.h"

namespace sg::ogl::resource
{
    class Model;
    class ShaderManager;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Renderer;

    class SG_OGL_API Scene
    {
    public:
        using ModelContainer = std::vector<std::reference_wrapper<resource::Model>>;

        using CameraUniquePtr = std::unique_ptr<camera::LookAtCamera>;
        using CameraContainer = std::vector<CameraUniquePtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        /**
         * @brief Ctor.
         * @param t_name The unique name of this Scene.
         * @param t_shaderProgramName The name of the ShaderProgram.
         * @param t_shaderManager Reference to the ShaderManager
         * @param t_defaultCamera Reference to a default camera.
         * @param t_projectionMatrix The projection matrix.
         * @param t_renderer Pointer to a parent Renderer.
         */
        Scene(
            std::string t_name,
            std::string t_shaderProgramName,
            resource::ShaderManager& t_shaderManager,
            camera::LookAtCamera& t_defaultCamera,
            glm::mat4& t_projectionMatrix,
            Renderer* t_renderer = nullptr
        );

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        /**
         * @brief Get scene's renderer.
         * @return Nullptr or pointer to a parent renderer.
         */
        Renderer* GetRenderer() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        /**
         * @brief Set the renderer of the scene.
         * @param t_renderer Pointer to a parent renderer.
         */
        void SetRenderer(Renderer* t_renderer);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * @brief Render the models.
         */
        void Render();

        //-------------------------------------------------
        // Models
        //-------------------------------------------------

        /**
         * @brief Get the whole model container.
         * @return Reference to the model container.
         */
        ModelContainer& GetModels() noexcept;

        /**
         * @brief Get the whole model container.
         * @return Reference to the model container.
         */
        const ModelContainer& GetModels() const noexcept;

        /**
         * @brief Add a model into model container.
         * @param t_model Reference to a model.
         */
        void AddModel(resource::Model& t_model);

        /**
         * @brief Remove model from the container.
         * @param t_model Reference to a model.
         */
        void RemoveModel(const resource::Model& t_model);

        /**
         * @brief Remove all the models from the scene.
         */
        void ClearModels();

        //-------------------------------------------------
        // Cameras
        //-------------------------------------------------

        /**
         * @brief Get the whole camera container.
         * @return Reference to the camera container.
         */
        CameraContainer& GetCameras() noexcept;

        /**
         * @brief Get the whole camera container.
         * @return Reference to the camera container.
         */
        const CameraContainer& GetCameras() const noexcept;

        /**
         * @brief Add a camera into camera container.
         * @param t_camera A camera unique pointer.
         */
        void AddCamera(CameraUniquePtr t_camera);

    protected:

    private:
        /**
         * @brief The unique name of this scene.
         */
        std::string m_name;

        /**
         * @brief The name of the ShaderProgram.
         */
        std::string m_shaderProgramName;

        /**
         * @brief Reference to the ShaderManager.
         */
        resource::ShaderManager& m_shaderManager;

        /**
         * @brief Reference to a default camera.
         */
        camera::LookAtCamera& m_defaultCamera;

        /**
         * @brief The projection matrix.
         */
        glm::mat4 m_projectionMatrix;

        /**
         * @brief Nullptr or raw pointer to the renderer in which this scene is executing.
         */
        Renderer* m_renderer{ nullptr };

        /**
         * @brief Model container.
         */
        ModelContainer m_models;

        /**
         * @brief Camera container.
         */
        CameraContainer m_cameras;
    };
}
