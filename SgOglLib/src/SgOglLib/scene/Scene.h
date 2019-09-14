#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <vector>
#include "Core.h"

namespace sg::ogl::light
{
    struct DirectionalLight;
    struct PointLight;
}

namespace sg::ogl::resource
{
    class Model;
    struct Material;
    class ShaderProgram;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;
    class Entity;

    class SG_OGL_API Scene
    {
    public:
        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;

        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;

        glm::mat4 projectionMatrix{ glm::mat4(1.0f) };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene();

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        camera::LookAtCamera& GetCurrentCamera() noexcept;
        const camera::LookAtCamera& GetCurrentCamera() const noexcept;

        light::DirectionalLight& GetDirectionalLight() noexcept;
        const light::DirectionalLight& GetDirectionalLight() const noexcept;

        light::PointLight& GetPointLight() noexcept;
        const light::PointLight& GetPointLight() const noexcept;

        bool IsDirectionalLight() const;
        bool IsPointLight() const;

        Node* GetRoot() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);

        //-------------------------------------------------
        // Vertex attribute
        //-------------------------------------------------

        static void SetNodeInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node);

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);

        Entity* CreateSkydomeEntity(
            const ModelSharedPtr& t_model,
            const glm::vec3& t_scale,
            resource::ShaderProgram& t_shaderProgram
        );

        Entity* CreateSkyboxEntity(
            uint32_t t_cubemapId,
            resource::ShaderProgram& t_shaderProgram,
            float t_size = 500.0f
        );

        void AddNodeToRoot(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render() const;
        void Render(Node* t_node) const;

    protected:

    private:
        CameraSharedPtr m_currentCamera;

        DirectionalLightSharedPtr m_directionalLight;
        PointLightSharedPtr m_pointLight;

        Node* m_rootNode{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node);
        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size);
    };
}
