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
    class Skybox;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::terrain
{
    class Terrain;
}

namespace sg::ogl::scene
{
    class Node;
    class Renderer;
    class SkyboxRenderer;
    class TerrainRenderer;

    class SG_OGL_API Scene
    {
    public:
        using RendererSharedPtr = std::shared_ptr<Renderer>;
        using SkyboxRendererSharedPtr = std::shared_ptr<SkyboxRenderer>;
        using TerrainRendererSharedPtr = std::shared_ptr<TerrainRenderer>;

        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;

        using SkyboxSharedPtr = std::shared_ptr<resource::Skybox>;
        using TerrainSharedPtr = std::shared_ptr<terrain::Terrain>;

        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        Scene(
            const RendererSharedPtr& t_renderer,
            const SkyboxRendererSharedPtr& t_skyboxRenderer,
            const TerrainRendererSharedPtr& t_terrainRenderer
        );

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Renderer& GetRenderer() noexcept;
        const Renderer& GetRenderer() const noexcept;

        SkyboxRenderer& GetSkyboxRenderer() noexcept;
        const SkyboxRenderer& GetSkyboxRenderer() const noexcept;

        TerrainRenderer& GetTerrainRenderer() noexcept;
        const TerrainRenderer& GetTerrainRenderer() const noexcept;

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
        void SetSkybox(const SkyboxSharedPtr& t_skybox);
        void SetTerrain(const TerrainSharedPtr& t_terrain);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);

        //-------------------------------------------------
        // Vertex attribute
        //-------------------------------------------------

        static void SetNodeInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node);

        //-------------------------------------------------
        // Scene objects (Nodes)
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);
        static Node* CreateSkydomeNode(const ModelSharedPtr& t_model, const glm::vec3& t_scale);
        static Node* CreateSkyboxNode();

        void AddNodeToRoot(Node* t_node) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void Render() const;
        void Render(Node* t_node) const;

    protected:

    private:
        RendererSharedPtr m_renderer;
        SkyboxRendererSharedPtr m_skyboxRenderer;
        TerrainRendererSharedPtr m_terrainRenderer;

        CameraSharedPtr m_currentCamera;

        SkyboxSharedPtr m_skybox;
        TerrainSharedPtr m_terrain;

        DirectionalLightSharedPtr m_directionalLight;
        PointLightSharedPtr m_pointLight;

        Node* m_rootNode{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node);
    };
}
