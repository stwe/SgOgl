#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Application.h"
#include "Log.h"
#include "Entity.h"
#include "TerrainComponent.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"
#include "component/Components.h"
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

        glm::vec4 GetCurrentClipPlane() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetPointLight(const PointLightSharedPtr& t_pointLight);
        void SetCurrentClipPlane(const glm::vec4& t_currentClipPlane);

        //-------------------------------------------------
        // Vertex attribute
        //-------------------------------------------------

        static void SetInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);

        //-------------------------------------------------
        // Scene objects
        //-------------------------------------------------

        static Node* CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material = nullptr);

        void AddWaterComponent(
            Entity* t_entity,
            const uint32_t t_reflectionTextureId,
            const uint32_t t_refractionTextureId,
            const uint32_t t_dudvTextureId,
            const uint32_t t_normalTextureId,
            const uint32_t t_depthTextureId
        ) const
        {
            auto waterComponentUniquePtr{ std::make_unique<component::WaterComponent>() };
            waterComponentUniquePtr->reflectionTextureId = t_reflectionTextureId;
            waterComponentUniquePtr->refractionTextureId = t_refractionTextureId;
            waterComponentUniquePtr->dudvTextureId = t_dudvTextureId;
            waterComponentUniquePtr->normalTextureId = t_normalTextureId;
            waterComponentUniquePtr->depthTextureId = t_depthTextureId;

            t_entity->AddComponent(Component::Type::WATER, std::move(waterComponentUniquePtr));
        }

        Entity* CreateGuiEntity(
            const std::string& t_textureFileName,
            const glm::vec2& t_position,
            const glm::vec2& t_scale,
            const std::string& t_shaderFolderName
        )
        {
            // add texture to the TextureManager and get the Id
            const auto textureId{ m_application->GetTextureManager().GetTextureIdFromPath(t_textureFileName) };

            // create a new entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateGuiEntity()] Null pointer.")

            auto meshUniquePtr{ std::make_unique<resource::Mesh>() };
            SG_OGL_CORE_ASSERT(meshUniquePtr, "[Scene::CreateGuiEntity()] Null pointer.")

            auto materialUniquePtr{ std::make_unique<resource::Material>() };
            SG_OGL_CORE_ASSERT(materialUniquePtr, "[Scene::CreateGuiEntity()] Null pointer.")

            std::vector<float> vertices{
                -1.0f,  1.0f,
                -1.0f, -1.0f,
                 1.0f,  1.0f,
                 1.0f, -1.0f
            };

            const buffer::BufferLayout bufferLayout{
                { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
            };

            // add vao
            meshUniquePtr->GetVao().AddVertexDataVbo(vertices.data(), static_cast<int32_t>(vertices.size()) / 2, bufferLayout);

            // set the texture id as mapKd
            materialUniquePtr->mapKd = textureId;

            // setup entity
            entity->mesh = std::move(meshUniquePtr);
            entity->material = std::move(materialUniquePtr);
            entity->SetParentScene(this);

            // setup for generating a transformation matrix
            entity->GetLocalTransform().position = glm::vec3(t_position.x, t_position.y, 0.0f);
            entity->GetLocalTransform().scale = glm::vec3(t_scale.x, t_scale.y, 1.0f);

            entity->SetDebugName("Gui");

            // add render component
            AddRenderComponent<component::RenderComponent, component::GuiRenderConfig>(entity, t_shaderFolderName);

            return entity;
        }

        Entity* CreateGuiEntity(
            const uint32_t t_textureId,
            const glm::vec2& t_position,
            const glm::vec2& t_scale,
            const std::string& t_shaderFolderName
        )
        {
            SG_OGL_CORE_ASSERT(t_textureId, "[Scene::CreateGuiEntity()] Invalid texture id.")

            // create a new entity
            auto* entity{ new Entity };
            SG_OGL_CORE_ASSERT(entity, "[Scene::CreateGuiEntity()] Null pointer.")

            auto meshUniquePtr{ std::make_unique<resource::Mesh>() };
            SG_OGL_CORE_ASSERT(meshUniquePtr, "[Scene::CreateGuiEntity()] Null pointer.")

            auto materialUniquePtr{ std::make_unique<resource::Material>() };
            SG_OGL_CORE_ASSERT(materialUniquePtr, "[Scene::CreateGuiEntity()] Null pointer.")

            std::vector<float> vertices{
                    -1.0f,  1.0f,
                    -1.0f, -1.0f,
                     1.0f,  1.0f,
                     1.0f, -1.0f
            };

            const buffer::BufferLayout bufferLayout{
                { buffer::VertexAttributeType::POSITION_2D, "aPosition" },
            };

            // add Vbo
            meshUniquePtr->GetVao().AddVertexDataVbo(vertices.data(), static_cast<int32_t>(vertices.size()) / 2, bufferLayout);

            // set the texture id as mapKd
            materialUniquePtr->mapKd = t_textureId;

            // setup entity
            entity->mesh = std::move(meshUniquePtr);
            entity->material = std::move(materialUniquePtr);
            entity->SetParentScene(this);

            // setup for generating a transformation matrix
            entity->GetLocalTransform().position = glm::vec3(t_position.x, t_position.y, 0.0f);
            entity->GetLocalTransform().scale = glm::vec3(t_scale.x, t_scale.y, 1.0f);

            // add render component
            AddRenderComponent<component::RenderComponent, component::GuiRenderConfig>(entity, t_shaderFolderName);

            return entity;
        }

        /*
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
            AddRenderComponent<component::RenderComponent, component::ModelRenderConfig>(entity, t_shaderName);

            // add terrain component
            AddTerrainComponent(entity, t_terrain);

            return entity;
        }
        */

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

        glm::vec4 m_currentClipPlane{ glm::vec4(0.0f) };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        static void StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Entity* t_entity);

        static std::vector<glm::vec3> CreateSkyboxVertices(float t_size);

        template <typename TRenderComponent, typename TRenderConfig>
        void AddRenderComponent(Entity* t_entity, const std::string& t_shaderFolder) const
        {
            auto renderComponentUniquePtr{ std::make_unique<TRenderComponent>() };
            SG_OGL_CORE_ASSERT(renderComponentUniquePtr, "[Scene::AddRenderComponent()] Null pointer.")

            auto renderConfigUniquePtr{ std::make_unique<TRenderConfig>(t_shaderFolder) };
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
