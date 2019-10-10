#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>
#include "Application.h"
#include "Log.h"
#include "Entity.h"
#include "RenderComponent.h"
#include "TerrainComponent.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "resource/ModelManager.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"
#include "terrain/Terrain.h"

namespace sg::ogl::light
{
    struct DirectionalLight;
    struct PointLight;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::scene
{
    class Node;

    class SG_OGL_API Scene
    {
    public:
        using ModelSharedPtr = std::shared_ptr<resource::Model>;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;

        using CameraSharedPtr = std::shared_ptr<camera::LookAtCamera>;

        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;
        using PointLightSharedPtr = std::shared_ptr<light::PointLight>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        explicit Scene(Application* t_application);

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

        Application* GetApplicationContext() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);

        //-------------------------------------------------
        // Vertex attribute
        //-------------------------------------------------

        static void SetInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);

        template <typename T>
        Entity* CreateModelEntity(
            const std::string& t_modelPath,
            const std::string& t_shaderName,
            const MaterialSharedPtr t_alternativeMaterial = nullptr
        )
        {
            // add shader program to the ShaderManager
            m_application->GetShaderManager()->AddShaderProgram<T>(t_shaderName);

            // add model to the ModelManager
            const auto model{ m_application->GetModelManager()->GetModelFromPath(t_modelPath) };

            // create entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateModelEntity()] Null pointer.")

            // get model meshes
            const auto& meshes{ model->GetMeshes() };

            // create a child entity for each mesh
            if (meshes.size() > 1)
            {
                // for each mesh
                for (const auto& mesh : meshes)
                {
                    // create a child
                    auto* child{ new Entity };
                    SG_OGL_CORE_ASSERT(child, "[Scene::CreateModelEntity()] Null pointer.")

                    // set mesh && material
                    child->mesh = mesh;
                    child->material = t_alternativeMaterial ? t_alternativeMaterial : mesh->GetDefaultMaterial();
                    child->SetParentScene(this);

                    // add a render component
                    AddRenderComponent<RenderComponent, DefaultRenderConfig>(child, t_shaderName);

                    // add entity as child
                    entity->AddChild(child);
                }
            }
            else
            {
                entity->mesh = meshes[0];
                entity->material = t_alternativeMaterial ? t_alternativeMaterial : meshes[0]->GetDefaultMaterial();
                entity->SetParentScene(this);

                AddRenderComponent<RenderComponent, DefaultRenderConfig>(entity, t_shaderName);
            }

            return entity;
        }

        template <typename T>
        Entity* CreateSkydomeEntity(const std::string& t_modelPath, const std::string& t_shaderName)
        {
            // add shader program to the ShaderManager
            m_application->GetShaderManager()->AddShaderProgram<T>(t_shaderName);

            // add model to the ModelManager
            const auto model{ m_application->GetModelManager()->GetModelFromPath(t_modelPath) };
            SG_OGL_CORE_ASSERT(model->GetMeshes().size() == 1, "[Scene::CreateSkydomeEntity()] Invalid number of meshes.")

            // create entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateSkydomeEntity()] Null pointer.")

            entity->mesh = model->GetMeshes()[0];
            entity->material = model->GetMeshes()[0]->GetDefaultMaterial();
            entity->GetLocalTransform().scale = glm::vec3(m_application->GetProjectionOptions().farPlane * 0.5f);
            entity->SetParentScene(this);

            // add render component
            AddRenderComponent<RenderComponent, DefaultRenderConfig>(entity, t_shaderName);

            return entity;
        }

        template <typename T>
        Entity* CreateSkyboxEntity(
            const std::vector<std::string>& t_textureFileNames,
            const std::string& t_shaderName,
            const float t_size = 500.0f
        )
        {
            // add shader program to the ShaderManager
            m_application->GetShaderManager()->AddShaderProgram<T>(t_shaderName);

            // add a cubemap to the TextureManager
            const auto cubemapId{ m_application->GetTextureManager()->GetCubemapId(t_textureFileNames) };

            // create entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateSkyboxEntity()] Null pointer.")

            auto meshUniquePtr{ std::make_unique<resource::Mesh>() };
            SG_OGL_CORE_ASSERT(meshUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

            auto materialUniquePtr{ std::make_unique<resource::Material>() };
            SG_OGL_CORE_ASSERT(materialUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

            auto vertices{ CreateSkyboxVertices(t_size) };

            const buffer::BufferLayout bufferLayout{
                { buffer::VertexAttributeType::POSITION, "vPosition" },
            };

            meshUniquePtr->Allocate(bufferLayout, &vertices, static_cast<int32_t>(vertices.size()));

            // set the cubemap id as mapKd
            materialUniquePtr->mapKd = cubemapId;

            entity->mesh = std::move(meshUniquePtr);
            entity->material = std::move(materialUniquePtr);
            entity->SetParentScene(this);

            // add render component
            AddRenderComponent<RenderComponent, SkyboxRenderConfig>(entity, t_shaderName);

            return entity;
        }

        template <typename T, typename N, typename S>
        Entity* CreateTerrainEntity(const std::shared_ptr<terrain::Terrain>& t_terrain, const std::string& t_shaderName)
        {
            // add shader program to the ShaderManager
            m_application->GetShaderManager()->AddShaderProgram<T>(t_shaderName);

            // add compute shader to the ShaderManager
            m_application->GetShaderManager()->AddComputeShaderProgram<N>(t_terrain->GetTerrainOptions().normalmap.computeShaderName);
            m_application->GetShaderManager()->AddComputeShaderProgram<S>(t_terrain->GetTerrainOptions().splatmap.computeShaderName);

            // create entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateTerrainEntity()] Null pointer.")

            entity->mesh = t_terrain->GetMesh();
            SG_OGL_CORE_ASSERT(entity->mesh, "[Scene::CreateTerrainEntity()] Null pointer.")

            entity->material = nullptr;
            entity->SetParentScene(this);

            // add render component
            AddRenderComponent<RenderComponent, DefaultRenderConfig>(entity, t_shaderName);

            // add terrain component
            AddTerrainComponent(entity, t_terrain);

            return entity;
        }

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

        Application* m_application{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);

        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size);

        template <typename TRenderComponent, typename TRenderConfig>
        void AddRenderComponent(Entity* t_entity, const std::string& t_shaderName) const
        {
            auto renderComponentUniquePtr{ std::make_unique<TRenderComponent>() };
            SG_OGL_CORE_ASSERT(renderComponentUniquePtr, "[Scene::AddRenderComponent()] Null pointer.")

            auto renderConfigUniquePtr{ std::make_unique<TRenderConfig>(m_application->GetShaderManager()->GetShaderProgram(t_shaderName)) };
            SG_OGL_CORE_ASSERT(renderConfigUniquePtr, "[Scene::AddRenderComponent()] Null pointer.")

            renderComponentUniquePtr->SetRenderConfig(std::move(renderConfigUniquePtr));

            t_entity->AddComponent(Component::Type::RENDERER, std::move(renderComponentUniquePtr));
        }

        void AddTerrainComponent(Entity* t_entity, const std::shared_ptr<terrain::Terrain>& t_terrain) const
        {
            auto terrainComponentUniquePtr{ std::make_unique<TerrainComponent>(t_terrain) };
            SG_OGL_CORE_ASSERT(terrainComponentUniquePtr, "[Scene::AddTerrainComponent()] Null pointer.")

            t_entity->AddComponent(Component::Type::TERRAIN, std::move(terrainComponentUniquePtr));
        }
    };
}
